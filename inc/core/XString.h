#ifndef _X_STRING_H_
#define _X_STRING_H_

typedef char		TAnsiChar;

#ifdef _WIN32
typedef wchar_t TUnicodeChar;
#else
typedef unsigned short TUnicodeChar;
#endif


#ifdef _UNICODE
typedef TUnicodeChar		TChar;
#else
typedef TAnsiChar		TChar;
#endif


#define _MAKE_ASCII(x) (x)


#define _MAKE_UNICODE(x) (L##x)


#ifdef _UNICODE
#define T _MAKE_UNICODE
#else
#define T _MAKE_ASCII
#endif


class XString
{
public:
	
	XString();
	XString(XString& String);
	XString(TChar *String);
#ifdef _UNICODE
	XString(TAnsiChar *String);
#else
	XString(TUnicodeChar *String);
#endif
	virtual ~XString();
	TChar *GetString();
	XS32 GetLength();
	XBOOL IsNull();
	XBOOL IsValidIndex(XS32 Index);
	XS32 Compare(XString& String, XBOOL IgnoreCase=false);
	XBOOL Find(XString& Str, XS32& Pos, XBOOL IgnoreCase=false);
	void Delete(XS32 Pos, XS32 Count);
	void Insert(XS32 Pos, TChar c);
	void Insert(XS32 Pos,  XString& String);
	XString GetSubString(XS32 Start, XS32 Count, XString& Dest);
	void VarArg(TChar *Format, ...);
	void EatLeadingWhitespace();
	void EatTrailingWhitespace();
	TAnsiChar *ToAnsi(TAnsiChar *Buffer, XS32 BufferLen);
	TUnicodeChar *ToUnicode(TUnicodeChar *Buffer, XS32 BufferLen);
	XS32 ToInt();
	void operator =( XString& String);
	void operator =( TChar *String);
#ifdef _UNICODE
	void operator =( TAnsiChar *String);
#else
	void operator =( TUnicodeChar *String);
#endif

	friend XString operator +( XString& Str1,  XString& Str2);
	void operator +=( XString& String);
	operator TChar *();
	TChar& XString::operator [](XS32 Pos);
	
	XBOOL operator < ( XString& Str);	
	XBOOL operator > ( XString& Str);	
	XBOOL operator <=( XString& Str);	
	XBOOL operator >=( XString& Str);	
	XBOOL operator ==( XString& Str);	
	XBOOL operator !=( XString& Str);	
	XBOOL operator < ( TChar *Chr);		
	XBOOL operator > ( TChar *Chr);		
	XBOOL operator <=( TChar *Chr);		
	XBOOL operator >=( TChar *Chr);		
	XBOOL operator ==( TChar *Chr);		
	XBOOL operator !=( TChar *Chr);		

protected:
	void Optimize();
	void Grow(XS32 Num);
	void AssignFrom(XString& Str);
	void AssignFrom( TChar *Str);

	static TChar *AllocStr(XS32 Size);
	static void FreeStr(TChar *Ptr);

protected:
	TChar					*Text;			
	XS32						Size;		
	XS32						Len;				
};

XString operator +( XString& Str1,  XString& Str2);

#endif
