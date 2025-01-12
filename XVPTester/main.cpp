
#include <main.h>
#include <mainState.h>
#include <xvpsdk.h>


XU32 FullScreenModeCallback(XFullScreenModeDesc* pFullScreenModes,XU32 numFullScreenModes)
{
	/*char buf[4096] = {0};
	for (XU32 i = 0;i < numFullScreenModes;++i)
	{
		char temp[1024];
		sprintf(temp,"%dx%d\n",pFullScreenModes[i].width,pFullScreenModes[i].height);
		X_DEBUG_LOG_LN(temp,X_DLL_INFO);
	}*/

	return 0;//numFullScreenModes-1;
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("XVP") ;
    MSG          msg ;
    WNDCLASS     wndclass ;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc   = X_WndProc;
    wndclass.cbClsExtra    = 0 ;
    wndclass.cbWndExtra    = 0 ;
    wndclass.hInstance     = hInstance ;
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
    wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

    RegisterClass(&wndclass);

	XU32 initialWidth =  1440;
	XU32 initialHeight = 900;
     
	hwnd = CreateWindow (szAppName,                     // window class name
						TEXT ("XVP"),					// window caption
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
                        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,						// window style
						0,								// initial x position
						0,								// initial y position
						initialWidth,					// initial width 
						initialHeight,					// initial height
						NULL,							// parent window handle
						NULL,							// window menu handle
						hInstance,						// program instance handle
						NULL) ;							// creation parameters

		/*hwnd2 = CreateWindow (szAppName,                     // window class name
						TEXT ("XVP2"),					// window caption
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
                        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,						// window style
						initialWidth,								// initial x position
						0,								// initial y position
						initialWidth/2,					// initial width 
						initialHeight/2,				// initial height
						NULL,							// parent window handle
						NULL,							// window menu handle
						hInstance,						// program instance handle
						NULL) ;							// creation parameters

 */
    // Get current flag
	//XS32 tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

	// Turn on leak-checking bit
	//tmpFlag |= _CRTDBG_LEAK_CHECK_DF;


	// Set flag to the new value
	//_CrtSetDbgFlag( tmpFlag );

	//_crtBreakAlloc = 991;
	
	

	//ShowCursor(false);
	ShowWindow(hwnd,iCmdShow) ;
  	UpdateWindow(hwnd) ;
 
	//ShowCursor(false);
	//ShowWindow(hwnd2,iCmdShow) ;
  	//UpdateWindow(hwnd2) ;

	if (XVP::init(hInstance) != X_OK)
	{
		MessageBoxA(NULL,"XVP failed to initialize.","",MB_OK);
		return -1;
	}

	//Initialize physics
	if (XPhysXController::initPhysics() != X_OK)
	{
		X_ASSERT_FORCED("Failed to start physics engine");
		return -1;
	}

	//Create a render context
	XRenderContextDesc rcDesc;
	rcDesc.pWindowHandle = hwnd;
	rcDesc.width = initialWidth;
	rcDesc.height = initialHeight;
	rcDesc.format = X_DFT_R8G8B8A8_UNORM;
	rcDesc.createDebugScene = true;
	//rcDesc.useDepthStencil = true;
	//rcDesc.depthStencilFormat = X_DFT_D24_UNORM_S8_UINT;
	//rcDesc.enableClear = false;
	if (XVP::GraphicsMgr.createRenderContext(rcDesc,rcHandle) != X_OK)
	{
		MessageBoxA(NULL,"Could not create render context.","",MB_OK);
		return -1;
	}

	//Create a render context 2
	/*XHandle rcHandle2;
	if (XVP::GraphicsMgr.createRenderContext(hwnd2,
											 initialWidth/2,
											 initialHeight/2,
											 X_DFT_R8G8B8A8_UNORM,
											 rcHandle2) != X_OK)
	{
		MessageBoxA(NULL,"Could not create render context 2.","",MB_OK);
		return -1;
	}*/

	XVP::GraphicsMgr.setRenderContextClearColor(rcHandle,XColor(.8f,.8f,.8f,1.0f));
	//XVP::GraphicsMgr.setClearColor(rcHandle2,XColor(0,1,0,1.0f));


	XVP::pushPreUpdateState(MainState::instance());

	//Register available shader controllers
	XVP::ShaderFactory.registerObject<XWhiteSC>("White.fx");
	XVP::ShaderFactory.registerObject<XVertexColorSC>("Vertex Color.fx");
	XVP::ShaderFactory.registerObject<XMaterialColorSC>("Material Color.fx");
	XVP::ShaderFactory.registerObject<XParticleSystemBlinnPhongSC>("Particle System-Blinn Phong.fx");
	XVP::ShaderFactory.registerObject<XParticleSystemNoLightSC>("Particle System-No Light.fx");
	XVP::ShaderFactory.registerObject<XModelBlinnPhongSC>("Model-Blinn Phong.fx");
	XVP::ShaderFactory.registerObject<XModelNoLightSC>("Model-No Light.fx");
	XVP::ShaderFactory.registerObject<XDistortionSC>("Model-Distortion.fx");
	
	while (true)
	{	
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			XVP::update();		
		}
	}
	
	XOSMgr::dumpStatistics();

	MainState::instance()->destroy();

	//Shutdown physics
	XPhysXController::destroyPhysics();

	XVP::destroy();

#ifndef XVP_FINAL
	XOSMgr::dumpAllocations();
#endif

	return (int)msg.wParam ;
}


LRESULT CALLBACK X_WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
	RECT r;
	POINTS p;
	

	switch (message)
    {
    case WM_CREATE:
        break;
	case WM_MOVE:
		break;
	case WM_EXITSIZEMOVE:

		if (XVP::isInitialized())
		{
			GetClientRect( hwnd, &r );
			
			XHandle thisRenderContext;
			char buf[256];
			GetWindowText(hwnd,buf,255);
			if (strcmp(buf,"XVP") == 0)
				thisRenderContext = 1;
			else
				thisRenderContext = 2;

			if ((r.right - r.left != XVP::GraphicsMgr.getRenderContextWidth(thisRenderContext)) ||
				(r.bottom - r.top != XVP::GraphicsMgr.getRenderContextHeight(thisRenderContext)))
			{
				//Check size so we don't try to go to small
				if ((r.right > 32) && (r.bottom > 32))
				{
					XVP::GraphicsMgr.setRenderContextClearColor(thisRenderContext,XColor(1,1,1,1));
					XVP::GraphicsMgr.resizeWindow(thisRenderContext,r.right,r.bottom);
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		p = MAKEPOINTS(lParam);

		//sprintf(buf,"%d %d\n",p.x,p.y);
		//OutputDebugString(buf);

		break;
	case WM_ACTIVATE:
		break;
    case WM_PAINT:
        break;
	case WM_KEYDOWN:
		break;
	case WM_DESTROY:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}

