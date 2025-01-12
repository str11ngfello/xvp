//*****************************************************************************
//*****************************************************************************
//     _  __         _    ______ 
//    | |/ /        | |  / / __ \
//    |   / ______  | | / / /_/ /
//   /   | /_____/  | |/ / ____/ 
//  /_/|_|          |___/_/       
//
//	Extensible Visualization Platform (X-VP) (c) 
//	Copyright 2000. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XRsa.cpp
//	Author:			Lynn Duke
//	Creation Date:	9-4-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XU8 primes[X_RSA_NUM_PRIMES] = {17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,
								 83,89,97,101,103,107,109,113,127,131,137,139,149,
								 151,157,163,167,173,179,181,191,193,197,199};



//*****************************************************************************
/**
Constructor for the RSA encryptor/decryptor.
*/
//*****************************************************************************
XRsa::XRsa()
{
	m_P = primes[XGenRandUINT32() % X_RSA_NUM_PRIMES];

	m_Q = m_P;
	while (m_Q == m_P)
		m_Q = primes[XGenRandUINT32() % X_RSA_NUM_PRIMES];
		
	m_N   = m_P * m_Q;
	m_PHI = (m_P - 1) * (m_Q - 1);
	m_E   = findE(m_PHI,m_P,m_Q);
	m_D   = extend(m_E,m_PHI);
}



//*****************************************************************************
//*****************************************************************************
int XRsa::findGreatestCommonDenomInternal(int m,int n)
{
	int a = m % n;

	while (a != 0)
	{
		m = n;
		n = a;
		a = m % n;
	}

	return n;
}



//*****************************************************************************
/**
Finds the greatest common denominator between two numbers.

@param m	First number.
@param n	Second number.
@return The greatest common denominator.
*/
//*****************************************************************************
int XRsa::findGreatestCommonDenom(int m,int n) 
{
   if (m == 0)
	   return n;
   if (n == 0)
	   return m;
   if (m < 0)
	   return findGreatestCommonDenom(-m,n);
   if (n < 0)
	   return findGreatestCommonDenom(m,-n);
   if (m > n)
	   return findGreatestCommonDenomInternal(n,m);

   return findGreatestCommonDenomInternal(m,n);
}



//*****************************************************************************
//*****************************************************************************
int XRsa::findE(int PHI,int P,int Q)
{
	int gcd = 0;
	int e = 2;
	while (gcd != 1) 
	{
		e = e + 1;
		gcd = findGreatestCommonDenom(e,m_PHI);
		m_PHI = (m_P - 1) * (m_Q - 1);
	}
		
	return e;
}



//*****************************************************************************
//*****************************************************************************
int XRsa::extend(int E,int PHI) 
{
	int u1 = 1;
	int u2 = 0;
	int u3 = PHI;
	int v1 = 0;
	int v2 = 1;
	int v3 = E;
	int t1,t2,t3,q;
	int inverse;

	while (v3 != 0) 
	{
		q = (int)floor((XF32)(u3 / v3));
	
		t1 = u1 - (q * v1);
		t2 = u2 - (q * v2);
		t3 = u3 - (q * v3);

		u1 = v1;
		u2 = v2;
		u3 = v3;

		v1 = t1;
		v2 = t2;
		v3 = t3;
	}
		
	int vv = u2;

	if (vv < 0) 
		inverse = vv + m_PHI;
	else 
		inverse = vv;

	return inverse;
}



//*****************************************************************************
//*****************************************************************************
int XRsa::powMod(int n,int e,int m)
{
	if (m == 0 || e < 0) 
		return 0;
   
	int res = 1;
	int pow = n;
	int e1 = e;
	while (e1 != 0) 
	{
		int d = e1 % 2;
		e1 = (int)floor((XF32)(e1 / 2));
	
		if (d == 1)
			res = (res * pow) % m;
		
		pow = (pow * pow) % m;
	}

	if (res < 0)
		res += m;
		 
	return res;
}



//*****************************************************************************
/**
Encrypts data.  This function takes a stream of data in UINT8s and outputs
an encrypted version of the same data in a stream of UINT16s.  Note that the
data is doubled in size.

@param pInputBuffer	A pointer to the byte buffer to encrypt.  There is no limit
					on the size of buffer for this parameter.
@param inputBufferLength	The length of the input buffer in bytes.
@param pOutputBuffer	A pointer to the buffer where the encrypted data will
						be stored.  This buffer must be the same size as the
						input buffer.  For example, if the input buffer is
						10 UINT8s, this buffer should be 10 UINT16s.  If the input
						buffer is 15 UINT8s long, the output buffer must be
						15 UINT16s long.  This means that encrypted
						data will be twice the size (in bytes) of its descrypted
						counterpart.
@param keyPart1	First half of the RSA key.
@param keyPart2	Second half of the RSA key.
@return None.
*/
//*****************************************************************************
void XRsa::encrypt(XS8* pInputBuffer,XU32 inputBufferLength,XU16* pOutputBuffer,
				   int keyPart1,int keyPart2)				 
{
	for (XU32 i = 0;i < inputBufferLength;++i)
		pOutputBuffer[i] = powMod(pInputBuffer[i],keyPart1,keyPart2);
}



//*****************************************************************************
/**
Encrypts data.  This function takes a stream of data in UINT8s and outputs
an encrypted version of the same data in a stream of UINT16s.

@param pInputBuffer	A pointer to the byte buffer to encrypt.  There is no limit
					on the size of buffer for this parameter.
@param inputBufferLength	The length of the input buffer in bytes.
@param pOutputBuffer	A pointer to the buffer where the encrypted data will
						be stored.  This buffer must be the same size as the
						input buffer.  For example, if the input buffer is
						10 UINT8s, this buffer should be 10 UINT16s.  If the input
						buffer is 15 UINT8s long, the output buffer must be
						15 UINT16s long.  This means that encrypted
						data will be twice the size (in bytes) of its descrypted
						counterpart.
@return None.
*/
//*****************************************************************************
void XRsa::encrypt(XS8* pInputBuffer,XU32 inputBufferLength,XU16* pOutputBuffer)
{
	for (XU32 i = 0;i < inputBufferLength;++i)
		pOutputBuffer[i] = powMod(pInputBuffer[i],m_E,m_N);
}



//*****************************************************************************
/**
Decrypts data.  This function takes a stream of data in USHORTS and outputs
an decrypted version of the same data in a stream of USHORTS.

@param pInputBuffer	A pointer to the XU16 buffer to decrypt.  There is no limit
					on the size of buffer for this parameter.
@param inputBufferLength	The length of the input buffer in USHORTS.
@param pOutputBuffer	A pointer to the buffer where the decrypted data will
						be stored.  This buffer must be the same size as the
						input buffer.  For example, if the input buffer is
						10 UINT16s, this buffer should be 10 XU8.  If the input
						buffer is 15 UINT16s long, the output buffer must be
						15 XU8 long.  This means that decrypted
						data will be half the size of its encrypted
						counterpart.
@return None.
*/
//*****************************************************************************
void XRsa::decrypt(XU16* pInputBuffer,XU32 inputBufferLength,XS8* pOutputBuffer)
{
	for (XU32 i = 0;i < inputBufferLength;++i)
		pOutputBuffer[i] = powMod(pInputBuffer[i],m_D,m_N);
}

