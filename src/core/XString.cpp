#include <xvpsdk.h>




XString::XString()								
	{
		Text = NULL;
		Len = Size = 0;
	}
	XString::XString( XString& String)	
	{											
		AssignFrom(String);
	}
	XString::XString( TChar *String)		// Constructor building a string from a regular C string
	{											// of characters in native format.
		AssignFrom(String);
	}
#ifdef _UNICODE
	XString::XString( TAnsiChar *String)	// Constructor building a Unicode string from a regular C
	{											// string in ANSI format (used for doing conversions).
		if (String == NULL)						// Early out if string is NULL
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = strlen(String) + 1;				// Use ANSI strlen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (XS32 i=0 ; i<Len ; i++)			// Convert ANSI->Unicode char-by-char
			Text[i] = (TUnicodeChar)String[i];
		Text[Len] = _MAKE_ASCII('\0');
	}
#else
	XString::XString( TUnicodeChar *String)		// Constructor building an ANSI string from a regular C
	{													// string in Unicode format (used for doing conversions).
		if (String == NULL)						// Early out if string is NULL
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = wcslen(String) + 1;				// Use Unicode wcslen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (XS32 i=0 ; i<Len ; i++)			// Convert Unicode->ANSI char-by-char
			Text[i] = (TAnsiChar)String[i];
		Text[Len] = _MAKE_ASCII('\0');
	}
#endif
	XString::~XString()						// Destructor. The use of virtual is recommended in C++
	{												// for all destructors that can potentially be overloaded.
		if (Size && Text != NULL)			// Free the memory used by the string
			FreeStr(Text);
	}

	//=======================================================
	//
	// Accessors
	//
	TChar *XString::GetString() 		// Returns a pointer to the actual string data.
	{
		return Text;
	}
	 XS32 XString::GetLength() 	// Returns the length of the string currently
	{										// held by an instance of the string class.
		return Len;
	}
	 XBOOL XString::IsNull() 		// Returns true if a NULL string is held by an instance.
	{
		return (Text == NULL || !_tcslen(Text));
	}
	 XBOOL XString::IsValidIndex( XS32 Index) 	// Returns true if the character index
	{																// specified is within the bounds of the string.
		return (Index >= 0 && Index < Len);
	}

	//=======================================================
	//
	// Regular transformation functions
	//	
	XS32 XString::Compare( XString& String, XBOOL IgnoreCase) 	// Compares another string
	{																						// with this instance. Return
		if (IsNull() && !String.IsNull())										// values equals those of strcmp(),
			return 1;																	// ie -1, 0 or 1 (Less, Equal, Greater)
		else if (!IsNull() && String.IsNull())							// ... Trivial cases
			return -1;
		else if (IsNull() && String.IsNull())
			return 0;
		
		if (IgnoreCase)
			return _tcsicmp(Text, String.Text);
		else
			return _tcscmp(Text, String.Text);
	}
	XBOOL XString::Find(XString& Str, XS32& Pos, XBOOL IgnoreCase)		// Finds a substring within this string.
	{																					// Returns true if found (position in Pos).
		if (IsNull() || Str.IsNull())
			return false;

		
		if (IgnoreCase)
		{
			for (Pos=0 ; Pos<=(Len-Str.Len) ; Pos++)
			{
				if (_tcsnicmp(&Text[Pos], Str.Text, Str.Len) == 0)
					return true;
			}
		}
		else
		{
			for (Pos=0 ; Pos<=(Len-Str.Len) ; Pos++)
			{
				if (_tcsncmp(&Text[Pos], Str.Text, Str.Len) == 0)
					return true;
			}
		}
		

		return false;
	}
	void XString::Delete(XS32 Pos, XS32 Count)						// Deletes the specified number of characters,
	{																// starting at the specified position.
		if (Pos > Len || Count==0)							// Start is outside string or nothing to delete?
			return;
		if (Pos + Count > Len)								// Clamp Count to string length
			Count = (Len - Pos);

		// Characters are deleted by moving up the data following the region to delete.
		for (XS32 i=Pos ; i<Len-Count ; i++)
			Text[i] = Text[i+Count];

	   Len -= Count;
		Text[Len] = T('\0');
		Optimize();
	}
	void XString::Insert(XS32 Pos, TChar c)			// Inserts a character at the given position in the string.
	{
	   if (Pos<0 || Pos>Len)
			return;

		Grow(1);									// Grow the string by one byte to be able to hold character

		// Move down rest of the string.
		// Copying overlapping memory blocks requires the use of memmove() instead of memcpy().
		memmove((void *)&Text[Pos+1], ( void *)&Text[Pos], Len-Pos);
		Text[Pos] = c;
		Text[++Len] = T('\0');
	}
	void XString::Insert(XS32 Pos,  XString& String)		// Inserts a complete string at the given
	{																// location.
	   if (Pos<0 || Pos>Len || String.IsNull())
			return;

		TChar *New = AllocStr(String.Len + Len + 1);
		if (Pos > 0)									// Set the string portion before the inserted string
			_tcsncpy(New, Text, Pos);
		_tcsncpy(&New[Pos], String.Text, String.Len);			// Insert the string
		if (Len-Pos > 0)													// Insert rest of orignal string
			_tcsncpy(&New[Pos+String.Len], &Text[Pos], Len-Pos);

		AssignFrom(New);							// Copy new string back into stringobject
	}
	XString XString::GetSubString(XS32 Start, XS32 Count, XString& Dest)		// Crops out a substring.
	{
		if (!IsValidIndex(Start) || Count <= 0)		// Valid operation?
		{
			Dest = T("");
			return Dest;
		}

		TChar *Temp = AllocStr(Count + 1);
		_tcsncpy(Temp, &Text[Start], Count);
		Temp[Count] = T('\0');

		Dest = Temp;
		FreeStr(Temp);
		return Dest;
	}

	//=======================================================
	//
	// Special transformation functions
	//
	void XString::VarArg(TChar *Format, ...)		// Allows you to fill a string object with data in the
	{												// same way you use sprintf()
		TChar Buf[0x1000];      // Need lots of space
		va_list  argptr;

		va_start(argptr, Format);
#ifdef _UNICODE
		vswprintf(Buf, Format, argptr);
#else
		vsprintf(Buf, Format, argptr);
#endif
		va_end(argptr);

		Size = _tcslen(Buf) + 1;
		Len = _tcslen(Buf);
		FreeStr(Text);
		Text = AllocStr(Size);
	   if (Len > 0)
			_tcscpy(Text, Buf);
		else
			Text[0] = T('\0');
	}
	void XString::EatLeadingWhitespace()		// Convenient function that removes all whitespace
	{											// (tabs and spaces) at the beginning of a string.
		if (IsNull())
			return;
		XS32 i = 0;
	   for (i=0 ; i<Len ; i++)
		{
			if (Text[i] != 0x20 && Text[i] != 0x09)
				break;
		}
		Delete(0, i);
	}
	void XString::EatTrailingWhitespace()		// Convenient function that removes all whitespace
	{											// (tabs and spaces) from the end of a string.
		if (IsNull())
			return;
		XS32 i = 0;
	   for (i=Len-1 ; i>=0 ; i--)
		{
			if (Text[i] != 0x20 && Text[i] != 0x09)
				break;
		}
		Delete(i+1, Len-i-1);
	}

	//=======================================================
	//
	// Conversion functions
	//
	TAnsiChar *XString::ToAnsi(TAnsiChar *Buffer, XS32 BufferLen) 	// Converts the string to an ANSI string.
	{
		XS32 i, ConvertLen;

		if (BufferLen <= 0)
		{
			Buffer = NULL;
			return Buffer;
		}

		if (BufferLen >= Len)
			ConvertLen = Len;
		else
			ConvertLen = BufferLen-1;

		for (i=0 ; i<ConvertLen ; i++)
		{
#ifdef _UNICODE
			if (Text[i] > 255)		// If character is a non-ANSI Unicode character, fill with
				Buffer[i] = 0x20;		// space instead.
			else
#endif
				Buffer[i] = (TAnsiChar)Text[i];
		}
		Buffer[i] = _MAKE_ASCII('\0');
		return Buffer;
	}
	TUnicodeChar *XString::ToUnicode(TUnicodeChar *Buffer, XS32 BufferLen) 		// Converts the string
	{																								// to a Unicode string.
		XS32 i, ConvertLen;

		if (BufferLen <= 0)
		{
			Buffer = NULL;
			return Buffer;
		}

		if (BufferLen >= Len)
			ConvertLen = Len;
		else
			ConvertLen = BufferLen-1;

		for (i=0 ; i<ConvertLen ; i++)
			Buffer[i] = (TUnicodeChar)Text[i];
		Buffer[i] = _MAKE_UNICODE('\0');
	   return Buffer;
	}
	XS32 XString::ToInt() 								// Converts the string to an integer.
	{
		if (IsNull())
			return 0;
#ifdef _UNICODE
		return _wtoi(Text);						// Unicode version of atoi
#else
		return atoi(Text);						// ANSI version of atoi
#endif
	}

	//=======================================================
	//
	// Assignment operators
	//
	void XString::operator =( XString& String)		// Sets this string to the contents of another string.
	{
		AssignFrom(String);
	}
	void XString::operator =( TChar *String)		// Sets this string to the contents of a character array
	{														// in native format.
		AssignFrom(String);
	}
#ifdef _UNICODE
	void operator =( TAnsiChar *String)	// Sets this string to the contents of a character array
	{														// in ANSI format (only included in Unicode builds)
		if (String == NULL)
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = strlen(String) + 1;			// Use ANSI strlen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (XS32 i=0 ; i<Len ; i++)		// Convert ANSI->Unicode char-by-char
			Text[i] = (TUnicodeChar)String[i];
		Text[Len] = _MAKE_ASCII('\0');
	}
#else
	void XString::operator =( TUnicodeChar *String)	// Sets this string to the contents of a character array
	{															// in Unicode format (only included in ANSI builds)
		if (String == NULL)
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = wcslen(String) + 1;		// Use Unicode wcslen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (XS32 i=0 ; i<Len ; i++)		// Convert Unicode->ANSI char-by-char
			Text[i] = (TAnsiChar)String[i];
		Text[Len] = _MAKE_ASCII('\0');
	}
#endif

	//=======================================================
	//
	// Concatenation operators
	//
	XString operator +( XString& Str1,  XString& Str2);	// Concatenates two strings (see text)

	void XString::operator +=( XString& String)		// Adds another string to the end of the
	{															// current one.
		if (String.Len > 0)
		{
			Grow(String.Len);
			_tcsncpy(&Text[Len], String.Text, String.Len);
			Len += String.Len;
		}
	}

	//=======================================================
	//
	// Access operators
	//
	XString::operator TChar *() 					// Returns the address of the contained string.
	{
		return Text;
	}
	TChar& XString::operator [](XS32 Pos)	// Returns a character reference at a
	{													// specific location.
		if (Pos < 0)								// If underrun, just return first character
			return Text[0];
		else if (Pos >= Len)						// If overrun, expand string in accordance
		{
			Grow(Pos+2);
			return Text[Pos];
		}
		else											// Otherwise, just return character
			return Text[Pos];
	}

	//=======================================================
	//
	// Comparison operators (operates through Compare()).
	// Functions exist for comparing both string objects and character arrays.
	//
	XBOOL XString::operator < ( XString& Str) 	{ return (XBOOL)(Compare(Str) == -1);				}
	XBOOL XString::operator > ( XString& Str) 	{ return (XBOOL)(Compare(Str) == 1);					}
	XBOOL XString::operator <=( XString& Str) 	{ return (XBOOL)(Compare(Str) != 1);					}
	XBOOL XString::operator >=( XString& Str) 	{ return (XBOOL)(Compare(Str) != -1);				}
	XBOOL XString::operator ==( XString& Str) 	{ return (XBOOL)(Compare(Str) == 0);					}
	XBOOL XString::operator !=( XString& Str) 	{ return (XBOOL)(Compare(Str) != 0);					}
	XBOOL XString::operator < ( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) == -1);	}
	XBOOL XString::operator > ( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) == 1);		}
	XBOOL XString::operator <=( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) != 1);		}
	XBOOL XString::operator >=( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) != -1);	}
	XBOOL XString::operator ==( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) == 0);		}
	XBOOL XString::operator !=( TChar *Chr) 		{ return (XBOOL)(Compare(XString(Chr)) != 0);		}


	void XString::Optimize()			// Discards any unused space allocated for the string
	{
		Size = Len + 1;
		TChar *Temp = AllocStr(Size);
		_tcscpy(Temp, Text);
		FreeStr(Text);
		Text = Temp;
	}
	void XString::Grow(XS32 Num)		// Allocates some more memory so the string can hold an additional Num characters
	{
		Size += Num;
		TChar *Temp = AllocStr(Size);
		_tcscpy(Temp, Text);
		FreeStr(Text);
		Text = Temp;
	}
	void XString::AssignFrom(XString& Str)			// Does the hard work for all non-converting assignments
	{
		Size = Str.Size;
		Len = Str.Len;
		if (Size && Len)						// No point copying an empty string
		{
			Text = AllocStr(Size);
			_tcscpy(Text, Str.Text);
		}
		else
		{
			Text = NULL;
		}
	}
	void XString::AssignFrom( TChar *Str)	// Does the hard work for all non-converting assignments
	{
		if (Str == NULL)						// Early out if string is NULL
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}

		Size = _tcslen(Str) + 1;
		Len = Size-1;
		Text = AllocStr(Size);
		_tcscpy(Text, Str);
	}

	TChar *XString::AllocStr(XS32 Size)		// Allocates a new character array. You can modify this
	{												// function if you for instance use a custom memory manager.
		//return new TChar[Size];
		//return (TChar *)calloc(Size, sizeof(TChar));
		TChar* pMem = NULL;
		XNewArrayFill(pMem,TChar,Size,0,XOSMgr::getDefaultHeap());
		return pMem;
	}
	void XString::FreeStr(TChar *Ptr)		// Ditto
	{
		if (Ptr == NULL)
			return;
		//delete [] Ptr;
		XDeleteArray(Ptr);
	}




//=======================================================
//
// Concatenation function (must be global for reasons stated in the text)
//
XString operator +( XString& Str1,  XString& Str2)
{
	TChar *Temp = XString::AllocStr(Str1.Size + Str2.Size);		// Allocate memory for new string
	if (Str1.Len)																// Copy first string into dest
		_tcscpy(Temp, Str1.Text);
	if (Str2.Len)																// Copy second string into dest
		_tcscpy(&Temp[Str1.Len], Str2.Text);

   XString Result = Temp;
	return Result;
}

