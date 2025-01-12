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
//	Module Name:	XSmoothFilter.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-30-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SMOOTH_FILTER_H_
#define __X_SMOOTH_FILTER_H_

#include <XOSMgr.h>
#include <XMemBlockList.h>



//*****************************************************************************
//! Smoothing filter that will smooth out numerical input.
/**
The XSmoothFilter object can take a set of input numbers and calculate
a number that is the result of a smoothing algorithm applied to the input
numbers.  The smooth filter object maintains a list of numbers, called the
history list, that are smoothed out by taking large percentages of recent
numbers and increasingly smaller percentages of previous numbers.  You can "feed"
new numbers into the smoothing filter and the resulting "smoothed" value will be 
recalculated immediately. 

There are 3 variables used in the smoothing algorithm.  They are the history list,
smooth factor, and damp factor.  

1.  History List - A list that contains the input numbers in "most recent" to
"least recent" order. <br><br>
2.  Smooth Factor - This is the rate at which the weight of numbers decrease
with their age in the history list.<br><br>
3.  Damp Factor - A linear factor used to scale the smoothed output up or down.

Smooth filters can be used to filter any input stream of numbers.  For example,
you can use a smooth filter to filter input from hardware devices where the
input is jerky or erratic at times.  You could pump mouse cursor
positional data into a smooth filter and receive a nice steady stream of
input from the smooth filter, even if the user jerks the mouse around here and
there or the mouse device itself is erratic.
*/
//*****************************************************************************
class XSmoothFilter
{
private:
	XU32			m_historyListLength;
	XF32			m_averageTotal;
	XF32			m_currentAverage;
	XF32			m_currentWeight;
	XF32			m_smoothFactor;
	XF32			m_dampFactor;
	XMemBlockList<XF32>	m_historyList;
	XBOOL			m_inited;
	
public:
	//*****************************************************************************
	//Constructor/Destructor
	//*****************************************************************************
	XSmoothFilter();
	virtual ~XSmoothFilter();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XU32 listLength,XF32 smoothFactor,XF32 dampFactor);
	XStatus destroy();
	XF32 update(XF32 newValue);
	void setSmoothFactor(XF32 smoothFactor);
	XF32 getSmoothFactor();
	void setDampFactor(XF32 dampFactor);
	XF32 getDampFactor();
	void setListLength(XU32 lenth);
	XU32 getListLength();
};

#endif