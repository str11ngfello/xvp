#include <xvpsdk.h>


//*****************************************************************************
//Global variables specific to windows.The Global HWND variable is stored here.
//*****************************************************************************
HWND hwnd;
HWND hwnd2;

XHandle rcHandle;



//*****************************************************************************
//Defines for core windows code including WinMain and WndProc. You can modify
//both of these functions if needed.
//*****************************************************************************
int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
                    PSTR szCmdLine,
					int iCmdShow);

LRESULT CALLBACK X_WndProc (HWND hwnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam);

