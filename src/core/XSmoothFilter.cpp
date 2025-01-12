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
//	Creation Date:	12-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the smooth filter.  Initializes the smooth filter properties to
the following default values:<br><br>

<ul>
<li> History List Length = 10 </li>
<li> Smooth Factor = 0 </li>
<li> Damp Factor = 1 </li>
</ul>
*/
//*****************************************************************************
XSmoothFilter::XSmoothFilter()
{
	m_historyListLength = 10;
	m_smoothFactor = 0;
	m_dampFactor = 1;
	m_averageTotal = 0;
	m_currentAverage = 0;
	m_currentWeight = 0;	
}



//*****************************************************************************
/**
Destructor for the smooth filter.  
*/
//*****************************************************************************
XSmoothFilter::~XSmoothFilter()
{
}



//*****************************************************************************
/**
Constructs a smooth filter with the passed in parameters.

@param listLength	The size of the history list.  This number tells the
					smooth filter how many values to keep in the history list
					and therefore use in the smoothing calculation.  For example,
					if you set this parameter equal to 50, then each value
					passed to the smooth filter with an update() call will stay
					in the history list for 50 update() calls.
@param smoothFactor	The is the smooth factor.  This is the percentage of change
					in weighting used for each value in the history list.  For
					example, if the smooth factor is set equal to 0.5, and the
					history length is set equal to 10.  Then the most recent value
					will get taken at full weight, the previous value in the
					history list will be taken at the weight of the smooth factor
					multiplied by the current weight (0.5 * 1).  In other words
					the previous value in the history list will only count 
					towards the final value at half of its weight.  The next previous
					value in the history list will be taken at a weight of
					(0.5 * .5) = .25, and so on through the entire history list.
@param dampFactor	The dampFactor applies a linear dampening effect to the final
					value returned from an update() call.  This number can be used
					to scale the smoothed result up or down  By default this
					value is 1, no scale.
*/
//*****************************************************************************
XStatus XSmoothFilter::init(XU32 listLength,XF32 smoothFactor,XF32 dampFactor)
{
	m_historyListLength = listLength;
	m_smoothFactor = smoothFactor;
	m_dampFactor = dampFactor;
	m_averageTotal = 0;
	m_currentAverage = 0;
	m_currentWeight = 0;	
	m_inited = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSmoothFilter::destroy()
{
	if (!m_inited)
		return X_OK;

	m_historyList.clear();

	m_inited = false;

	return X_OK;
}



//*****************************************************************************
/**
Updates the smooth filter.  This function will feed a new value into the smooth
filter. The new value will be pushed onto the history list as the most recent
addition and the "smoothed" value of the entire history list will be recalculated
and returned.

@param newValue	The new value to add to the history list as the most recent.
@return A value resulting from the smoothing algorithm run over the entire
		history list of values.  This value is dampened by the dampFactor.
*/
//*****************************************************************************
XF32 XSmoothFilter::update(XF32 newValue)
{
	X_ASSERTM(m_inited,"XSmoothFilter used before initialized");

	//First, insert the new item if there is room
	if (m_historyList.getNumItems() < m_historyListLength)
		m_historyList.insertFront(newValue);
	else
	{
		m_historyList.removeBack();
		m_historyList.insertFront(newValue);
	}

	//Begin the averaging(smoothing) process...
	m_currentAverage = 0;
	m_averageTotal = 0;
	m_currentWeight = 1.0;
	XMemBlockList<XF32>::iterator i(m_historyList.begin());
	while (i.isValid())
	{
		m_currentAverage += i.getValue() * m_currentWeight;

		//Our total is also weighted
		m_averageTotal += 1 * m_currentWeight;

		//Calculate the weight for the next entry in the history buffer
		m_currentWeight *= m_smoothFactor;

		i.getNext();
	}

	//Calculate and return the final weighted value
	return (m_currentAverage / m_averageTotal) * m_dampFactor;
}



//*****************************************************************************
/**
Sets the smooth factor for the smooth filter.

@param smoothFactor	The is the smooth factor.  This is the percentage of change
					in weighting used for each value in the history list.  For
					example, if the smooth factor is set equal to 0.5, and the
					history length is set equal to 10.  Then the most recent value
					will get taken at full weight, the previous value in the
					history list will be taken at the weight of the smooth factor
					multiplied by the current weight (0.5 * 1).  In other words
					the previous value in the history list will only count 
					towards the final value at half of its weight.  The next previous
					value in the history list will be taken at a weight of
					(0.5 * .5) = .25, and so on through the entire history list.
@return None.
*/
//*****************************************************************************
void XSmoothFilter::setSmoothFactor(XF32 smoothFactor)
{
	m_smoothFactor = smoothFactor;
}



//*****************************************************************************
/**
Gets the smooth factor for the smooth filter.

@return	The is the smooth factor.  This is the percentage of change
		in weighting used for each value in the history list.  For
		example, if the smooth factor is set equal to 0.5, and the
		history length is set equal to 10.  Then the most recent value
		will get taken at full weight, the previous value in the
		history list will be taken at the weight of the smooth factor
		multiplied by the current weight (0.5 * 1).  In other words
		the previous value in the history list will only count 
		towards the final value at half of its weight.  The next previous
		value in the history list will be taken at a weight of
		(0.5 * .5) = .25, and so on through the entire history list.
*/
//*****************************************************************************
XF32 XSmoothFilter::getSmoothFactor()
{
	return m_smoothFactor;
}



//*****************************************************************************
/**
Sets the history list length.

@param length	The size of the history list.  This number tells the
				smooth filter how many values to keep in the history list
				and therefore use in the smoothing calculation.  For example,
				if you set this parameter equal to 50, then each value
				passed to the smooth filter with an update() call will stay
				in the history list for 50 update() calls.
@return None.
*/
//*****************************************************************************
void XSmoothFilter::setListLength(XU32 length)
{
	m_historyListLength = length;
}



//*****************************************************************************
/**
Gets the history list length.

@return The size of the history list.  This number tells the
		smooth filter how many values to keep in the history list
		and therefore use in the smoothing calculation.  For example,
		if you set this parameter equal to 50, then each value
		passed to the smooth filter with an update() call will stay
		in the history list for 50 update() calls.
*/
//*****************************************************************************
XU32 XSmoothFilter::getListLength()
{
	return m_historyListLength;
}



//*****************************************************************************
/**
Sets the damp factor for the smooth filter.  This value will scale the smoothed
result up or down.  By default, the damp factor is 1, no scaling.

@param dampFactor	The dampFactor applies a linear dampening effect to the final
					value returned from an update() call.  This number can be used
					to scale the smoothed result up or down  By default this
					value is 1, no scale.
@return None.
*/
//*****************************************************************************
void XSmoothFilter::setDampFactor(XF32 dampFactor)
{
	//if (dampFactor >= 1.0)
		m_dampFactor = dampFactor;
}



//*****************************************************************************
/**
Gets the damp factor for the smooth filter.  This value will scale the smoothed
result up or down.  By default, the damp factor is 1, no scaling.

@return The dampFactor applies a linear dampening effect to the final
		value returned from an update() call.  This number can be used
		to scale the smoothed result up or down  By default this
		value is 1, no scale.
*/
//*****************************************************************************
XF32 XSmoothFilter::getDampFactor()
{
	return m_dampFactor;
}