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
//	Module Name:	XProfiler.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-3-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XProfilerNode* XProfiler::m_pRoot;
XProfilerNode* XProfiler::m_pCurrentNode = NULL;
int XProfiler::m_frameCounter = 0;
XF32 XProfiler::m_resetTime = 0;
XUserInterface XProfiler::m_UI;
XF32 XProfiler::m_updateRate = 0;
XF32 XProfiler::m_timePassed = 0;
XUITextBox XProfiler::m_textBox;
char XProfiler::m_textBuffer[4096];
XBOOL XProfiler::m_active = false;
XBOOL XProfiler::m_inited = false;




//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::init(XHandle charSetHandle)
{
	if (charSetHandle == X_INVALID_HANDLE)
		return X_ERR_OPERATION_FAILED;

	//Allocate root profile node
	XNew(m_pRoot,XProfilerNode("ROOT",NULL),XOSMgr::getDebugHeap());
	//m_pRoot = (XProfilerNode*)XOSMgr::getDebugHeap()->Allocate(sizeof(XProfilerNode),__FILE__,__LINE__);
	//m_pRoot = new (m_pRoot) XProfilerNode("ROOT",NULL);
	m_pCurrentNode = m_pRoot;

	m_UI.setNativeResolution(1280,1024);
	
	XTextBoxDesc desc;
	desc.maxChars = 1000;
	desc.screenRect = XRect(0,0,1280,1024);
	desc.charSetHandle = charSetHandle;
	//m_textBox.init(&m_UI,0,desc,NULL,0,0); 
	XOSMgr::setPath(X_PATH_UI_TEMPLATE,"../system/profiler/");
	XOSMgr::setPath(X_PATH_TEXTURE,"../system/profiler/");
	if (m_textBox.init(&m_UI,0,XRect(0,0,1280,1024),"textbox.xml") != X_OK)
		return X_ERR_OPERATION_FAILED;

	m_textBox.setText("Lynn");

	m_inited = true;

	return X_OK;

}



//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::update(XF32 dt)
{
	if (!m_active)
		return X_ERR_NOT_READY;

	m_timePassed += dt;
			
	static XProfilerIterator* itr = getIterator();
	static char inputBuffer[10] ={0};
	XBOOL forceDisplay = false;

	itr->first();

	if (XVP::InputMgr.keyDown(X_KEY_SPACE))
	{
		if (strlen(inputBuffer) < 10)
		{
			if (XVP::InputMgr.keyDownOnce(X_KEY_0))
				strcat(inputBuffer,"0");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_1))
				strcat(inputBuffer,"1");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_2))
				strcat(inputBuffer,"2");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_3))
				strcat(inputBuffer,"3");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_4))
				strcat(inputBuffer,"4");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_5))
				strcat(inputBuffer,"5");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_6))
				strcat(inputBuffer,"6");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_7))
				strcat(inputBuffer,"7");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_8))
				strcat(inputBuffer,"8");
			else if (XVP::InputMgr.keyDownOnce(X_KEY_9))
				strcat(inputBuffer,"9");
		}
	}
	if (XVP::InputMgr.keyUpOnce(X_KEY_SPACE))
	{
		if (strlen(inputBuffer) > 0)
		{
			itr->enterChild(atoi(inputBuffer));
			inputBuffer[0] = '\0';
		}
		else
			itr->enterParent();
		forceDisplay = true;

	}


	if ((m_timePassed >= m_updateRate) || (forceDisplay))
	{
		char buf[512];
		int c = 0;
		m_textBuffer[0] = '\0';


		sprintf(buf,"PROFILER STATS\n");
		strcat(m_textBuffer,buf);
		sprintf(buf,"Marker Name    Max Capacity    Peak Usage    Current Usage     Avg Alloc\n");
		strcat(m_textBuffer,buf);
		sprintf(buf,"---------------------------------------------------------------------------\n");
		strcat(m_textBuffer,buf);
	/*
	int spacing = 20;
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList1.getName(),m_freeBlockList1.getMaxBlockCapacity(),m_freeBlockList1.getPeakBlocksInUse(),m_freeBlockList1.getNumBlocksInUse(),avgAllocation1);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList2.getName(),m_freeBlockList2.getMaxBlockCapacity(),m_freeBlockList2.getPeakBlocksInUse(),m_freeBlockList2.getNumBlocksInUse(),avgAllocation2);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList3.getName(),m_freeBlockList3.getMaxBlockCapacity(),m_freeBlockList3.getPeakBlocksInUse(),m_freeBlockList3.getNumBlocksInUse(),avgAllocation3);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList4.getName(),m_freeBlockList4.getMaxBlockCapacity(),m_freeBlockList4.getPeakBlocksInUse(),m_freeBlockList4.getNumBlocksInUse(),avgAllocation4);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList5.getName(),m_freeBlockList5.getMaxBlockCapacity(),m_freeBlockList5.getPeakBlocksInUse(),m_freeBlockList5.getNumBlocksInUse(),avgAllocation5);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList6.getName(),m_freeBlockList6.getMaxBlockCapacity(),m_freeBlockList6.getPeakBlocksInUse(),m_freeBlockList6.getNumBlocksInUse(),avgAllocation6);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList7.getName(),m_freeBlockList7.getMaxBlockCapacity(),m_freeBlockList7.getPeakBlocksInUse(),m_freeBlockList7.getNumBlocksInUse(),avgAllocation7);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList8.getName(),m_freeBlockList8.getMaxBlockCapacity(),m_freeBlockList8.getPeakBlocksInUse(),m_freeBlockList8.getNumBlocksInUse(),avgAllocation8);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
	sprintf (buf,"%-*s %9d %13d %16d %13d",spacing,m_freeBlockList9.getName(),m_freeBlockList9.getMaxBlockCapacity(),m_freeBlockList9.getPeakBlocksInUse(),m_freeBlockList9.getNumBlocksInUse(),avgAllocation9);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);

	X_DEBUG_LOG("\n\n",X_DLL_NONE);

*/

		while (!itr->isDone())
		{
			strcat(m_textBuffer,_itoa(c,buf,10));
			strcat(m_textBuffer," ");
			strcat(m_textBuffer,itr->getCurrentName());
			strcat(m_textBuffer," ");
			sprintf(buf,"%d",itr->getCurrentTotalCalls());
			strcat(m_textBuffer,buf);
			strcat(m_textBuffer," ");
			sprintf(buf,"%f",itr->getCurrentTotalTime());
			strcat(m_textBuffer,buf);
			strcat(m_textBuffer," ");
			sprintf(buf,"%f",itr->getCurrentTotalTime()/itr->getCurrentTotalCalls());
			strcat(m_textBuffer,buf);
			strcat(m_textBuffer," ");
			sprintf(buf,"%f",itr->getCurrentSlowestTime());
			strcat(m_textBuffer,buf);
			strcat(m_textBuffer," ");
			sprintf(buf,"%f",itr->getCurrentLastTime());
			strcat(m_textBuffer,buf);
			strcat(m_textBuffer,"\n");

			//f << c++ << " " << itr->getCurrentName() << "  " << itr->getCurrentTotalTime()<< std::endl;
			++c;
			itr->next();
		}

		m_textBox.setText(m_textBuffer);
		m_timePassed = 0;
	}
		
	//releaseIterator(itr);
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::destroy()
{
	#pragma message("implemenation needed")
	//XVP::removeUIFromScene(&m_UI);

	m_UI.destroy();
	m_textBox.destroy();

	XDelete(m_pRoot);
	//XOSMgr::getDebugHeap()->Deallocate(m_pRoot);

	m_active = false;

	m_inited = false;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::hide()
{
		#pragma message("implemenation needed")

	//XVP::removeUIFromScene(&m_UI);
	m_active = false;
	return X_OK;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::show()
{
		#pragma message("implemenation needed")
/*
	if (!XVP::isUIInScene(&m_UI))
		XVP::addUIToScene(&m_UI,X_CONSOLE_Z_ORDER);
*/
	m_active = true;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XProfiler::isVisible()
{
	return m_active;
}



//*****************************************************************************
//*****************************************************************************
XStatus XProfiler::setUpdateRate(XF32 updateRate)
{
	m_updateRate = updateRate;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XProfiler::getUpdateRate()
{
	return m_updateRate;
}



//*****************************************************************************
//*****************************************************************************
void XProfiler::startProfile(XS8* pName )
{
	if (pName != m_pCurrentNode->getName())
		m_pCurrentNode = m_pCurrentNode->getSubNode(pName);
	
	m_pCurrentNode->call();
}



//*****************************************************************************
//*****************************************************************************
void XProfiler::stopProfile()
{
	if (m_pCurrentNode->stop()) 
		m_pCurrentNode = m_pCurrentNode->getParent();
}



//*****************************************************************************
//*****************************************************************************
void XProfiler::reset()
{ 
	m_pRoot->reset(); 
	m_frameCounter = 0;
	m_resetTime = XOSMgr::getSysTime();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XProfiler::isInitialized()
{
	return m_inited;
}


	
//*****************************************************************************
//*****************************************************************************
void XProfiler::incrementFrameCounter()
{
	m_frameCounter++;
}



//*****************************************************************************
//*****************************************************************************
XF32 XProfiler::getTimeSinceReset()
{
	XF32 time;
	time = XOSMgr::getSysTime();
	time -= m_resetTime;
	return time; 
}

