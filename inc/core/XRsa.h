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
//	Module Name:	XRsa.h
//	Author:			Lynn Duke
//	Creation Date:	9-4-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RSA_H__
#define __X_RSA_H__

#include <XOSMgr.h>

#define X_RSA_NUM_PRIMES	40



//*****************************************************************************
//! Provides RSA encryption facilities.
/**
The XRsa object provides functionality to encrypt and decrypt data via RSA
encryption.       

See the member functions below for more information.
*/
//*****************************************************************************
class XRsa
{
private:
	int		m_N;
	int		m_PHI;
	int		m_E;
	int		m_D;
	int		m_Q;
	int		m_P;


	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	int findGreatestCommonDenomInternal(int m,int n);
	int findGreatestCommonDenom(int m,int n) ;
	int findE(int PHI,int P,int Q);
	int extend(int E,int PHI) ;
	int powMod(int n,int e,int m);

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRsa();
	void encrypt(XS8* pInputBuffer,XU32 inputBufferLength,XU16* pOutputBuffer,
				 int keyPart1,int keyPart2);
	void encrypt(XS8* pInputBuffer,XU32 inputBufferLength,XU16* pOutputBuffer);
	void decrypt(XU16* pInputBuffer,XU32 inputBufferLength,XS8* pOutputBuffer);
	void getPublicKey(int& keyPart1,int& keyPart2);
};

#endif