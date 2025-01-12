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
//	Module Name:	XUIProgressBar.h
//	Author:			Lynn Duke
//	Creation Date:	6-30-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_PROGRESS_BAR_H__
#define __X_UI_PROGRESS_BAR_H__




//*****************************************************************************
//! Progress bar control.
/**
The XUIProgressBar object functions similarly to progress bars in operating
systems such as Microsoft Windows.        

See the member functions below for more information.
*/
//*****************************************************************************
class XUIProgressBar : public XUIElement
{
protected:
	XF32		m_xScale;
	XF32		m_yScale;
	XSprite		m_trackSprite;
	XSprite		m_coverSprite;
	XRect		m_nativeCoverRect;
	XU32		m_coverSpriteWidth;
	XU32		m_coverSpriteHeight;
	XF32		m_percentage;
	XBOOL		m_coverFirst;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIProgressBar();
	virtual ~XUIProgressBar();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
							XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus renderCoverFirst(XBOOL coverFirst);
	XBOOL isCoverFirst();
	XStatus setPercentage(XF32 percent);
	XF32 getPercentage();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onResize(XF32 xScale,XF32 yScale);
};

#endif