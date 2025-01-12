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
//	Module Name:	XInputMgrTypes.h
//	Author:			Lynn Duke
//	Creation Date:	6-30-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************
#ifndef __X_INPUT_MGR_TYPES_H__
#define __X_INPUT_MGR_TYPES_H__

#define X_JOY_AXIS_EXTENT		1000
#define X_MAX_JOYSTICKS			4
#define X_MAX_JOY_BUTTONS		32
#define X_MAX_KEYS				256
#define X_MAX_MOUSE_BUTTONS		3



//*****************************************************************************
//*****************************************************************************
enum X_IDEVICE_TYPE
{
	X_IDEVICE_KEYBOARD		= 0x01,
	X_IDEVICE_MOUSE			= 0x02,
	X_IDEVICE_JOYSTICKS
};



//*****************************************************************************
//! Mouse button codes used for input driver routines.
/**
These defines specify mouse button codes for a standard computer mouse.
*/
//*****************************************************************************
enum X_MOUSE_BUTTON
{
	X_MB_LEFT,
	X_MB_RIGHT,
	X_MB_MIDDLE
};



//*****************************************************************************
//! Key codes used for input driver routines.
/**
These defines specify key codes for a standard computer keyboard.
*/
//*****************************************************************************
enum X_KEY
{
	X_KEY_ESCAPE =          0x01,	/*!< Escape key. */
	X_KEY_1 =               0x02,	/*!< Numerical 1 key. */
	X_KEY_2 =               0x03,	/*!< Numerical 2 key. */
	X_KEY_3 =               0x04,	/*!< Numerical 3 key. */
	X_KEY_4 =               0x05,	/*!< Numerical 4 key. */
	X_KEY_5 =               0x06,	/*!< Numerical 5 key. */
	X_KEY_6 =               0x07,	/*!< Numerical 6 key. */
	X_KEY_7 =               0x08,	/*!< Numerical 7 key. */
	X_KEY_8 =               0x09,	/*!< Numerical 8 key. */
	X_KEY_9 =               0x0A,	/*!< Numerical 9 key. */
	X_KEY_0 =               0x0B,	/*!< Numerical 0 key. */
	X_KEY_MINUS =           0x0C,   /*!< Dash/Minus key. */
	X_KEY_EQUALS =          0x0D,	/*!< Escape key. */
	X_KEY_BACK =            0x0E,   /*!< Backspace key. */
	X_KEY_TAB =             0x0F,	/*!< Tab key. */
	X_KEY_Q =               0x10,	/*!< Q/q key. */
	X_KEY_W =               0x11,	/*!< W/w key. */
	X_KEY_E =               0x12,	/*!< E/e key. */
	X_KEY_R =               0x13,	/*!< R/r key. */
	X_KEY_T =               0x14,	/*!< T/t key. */
	X_KEY_Y =               0x15,	/*!< Y/y key. */
	X_KEY_U =               0x16,	/*!< U/u key. */
	X_KEY_I =               0x17,	/*!< I/i key. */
	X_KEY_O =               0x18,	/*!< O/o key. */
	X_KEY_P =               0x19,	/*!< P/p key. */
	X_KEY_LBRACKET =        0x1A,	/*!< Left square bracket key. */
	X_KEY_RBRACKET =        0x1B,	/*!< Right square bracket key. */
	X_KEY_RETURN =          0x1C,   /*!< Enter/Return key. */
	X_KEY_LCONTROL =        0x1D,	/*!< Left control key. */
	X_KEY_A =               0x1E,	/*!< A/a key. */
	X_KEY_S =               0x1F,	/*!< S/s key. */
	X_KEY_D =               0x20,	/*!< D/d key. */
	X_KEY_F =               0x21,	/*!< F/f key. */
	X_KEY_G =               0x22,	/*!< G/g key. */
	X_KEY_H =               0x23,	/*!< H/h key. */
	X_KEY_J =               0x24,	/*!< J/j key. */
	X_KEY_K =               0x25,	/*!< K/k key. */
	X_KEY_L =               0x26,	/*!< L/l key. */
	X_KEY_SEMICOLON =       0x27,	/*!< Semicolon key. */
	X_KEY_APOSTROPHE =      0x28,	/*!< Apostrophe key. */
	X_KEY_GRAVE =           0x29,   /*!< Accent grave key. */
	X_KEY_LSHIFT =          0x2A,	/*!< Left shift key. */
	X_KEY_BACKSLASH =       0x2B,	/*!< Backslash key. */
	X_KEY_Z =               0x2C,	/*!< Z/z key. */
	X_KEY_X =               0x2D,	/*!< X/x key. */
	X_KEY_C =               0x2E,	/*!< C/c key. */
	X_KEY_V =               0x2F,	/*!< V/v key. */
	X_KEY_B =               0x30,	/*!< B/b key. */
	X_KEY_N =               0x31,	/*!< N/n key. */
	X_KEY_M =               0x32,	/*!< M/m key. */
	X_KEY_COMMA =           0x33,	/*!< Comma key. */
	X_KEY_PERIOD =          0x34,   /*!< Period key. */
	X_KEY_SLASH =           0x35,   /*!< Forward slash. */
	X_KEY_RSHIFT =          0x36,	/*!< Right shift key. */
	X_KEY_MULTIPLY =        0x37,   /*!< Multiply (*) key on numeric keypad. */
	X_KEY_LMENU =           0x38,   /*!< Left Alt key. */
	X_KEY_SPACE =           0x39,	/*!< Spacebar key. */
	X_KEY_CAPITAL =         0x3A,	/*!< Caps lock key. */
	X_KEY_F1 =              0x3B,	/*!< F1 function key. */
	X_KEY_F2 =              0x3C,	/*!< F2 function key. */
	X_KEY_F3 =              0x3D,	/*!< F3 function key. */
	X_KEY_F4 =              0x3E,	/*!< F4 function key. */
	X_KEY_F5 =              0x3F,	/*!< F5 function key. */
	X_KEY_F6 =              0x40,	/*!< F6 function key. */
	X_KEY_F7 =              0x41,	/*!< F7 function key. */
	X_KEY_F8 =              0x42,	/*!< F8 function key. */
	X_KEY_F9 =              0x43,	/*!< F9 function key. */
	X_KEY_F10 =             0x44,	/*!< F10 function key. */
	X_KEY_NUMLOCK =         0x45,	/*!< Numlock key. */
	X_KEY_SCROLL =          0x46,   /*!< Scroll Lock */
	X_KEY_NUMPAD7 =         0x47,	/*!< Escape key. */
	X_KEY_NUMPAD8 =         0x48,	/*!< Escape key. */
	X_KEY_NUMPAD9 =         0x49,	/*!< Escape key. */
	X_KEY_SUBTRACT =        0x4A,   /*!< Subtract (-) on numeric keypad */
	X_KEY_NUMPAD4 =         0x4B,	/*!< Escape key. */
	X_KEY_NUMPAD5 =         0x4C,	/*!< Escape key. */
	X_KEY_NUMPAD6 =         0x4D,	/*!< Escape key. */
	X_KEY_ADD =             0x4E,   /*!< Add (+) on numeric keypad. */
	X_KEY_NUMPAD1 =         0x4F,	/*!< Numerical 1 on keypad. */
	X_KEY_NUMPAD2 =         0x50,	/*!< Numerical 2 on keypad. */
	X_KEY_NUMPAD3 =         0x51,	/*!< Numerical 3 on keypad. */
	X_KEY_NUMPAD0 =         0x52,	/*!< Numerical 0 on keypad. */
	X_KEY_DECIMAL =         0x53,   /*!< Decimal (.) on numeric keypad. */
	X_KEY_OEM_102 =         0x56,   /* < > | on UK/Germany keyboards. */
	X_KEY_F11 =             0x57,	/*!< F11 function key. */
	X_KEY_F12 =             0x58,	/*!< F12 function key. */
	X_KEY_F13 =             0x64,   /*!< F13 function key. (NEC PC98) */
	X_KEY_F14 =             0x65,   /*!< F14 function key. (NEC PC98) */
	X_KEY_F15 =             0x66,   /*!< F15 function key. (NEC PC98) */
	X_KEY_KANA =            0x70,   /*!< (Japanese keyboard)            */
	X_KEY_ABNT_C1 =         0x73,   /*!< / ? on Portugese (Brazilian) keyboard. */
	X_KEY_CONVERT =         0x79,   /*!< Convert key. (Japanese keyboard) */
	X_KEY_NOCONVERT =       0x7B,   /*!< (Japanese keyboard)  */
	X_KEY_YEN =             0x7D,   /*!< (Japanese keyboard)            */
	X_KEY_ABNT_C2 =         0x7E,   /*!< Numpad . on Portugese (Brazilian) keyboard */
	X_KEY_NUMPADEQUALS =    0x8D,   /*!< Equals (=) on numeric keypad. (NEC PC98) */
	X_KEY_PREVTRACK =       0x90,   /*!< Previous Track. (X_KEY_CIRCUMFLEX on Japanese keyboard) */
	X_KEY_AT =              0x91,   /*!< AT key. (NEC PC98) */
	X_KEY_COLON =           0x92,   /*!< Colon key. (NEC PC98) */
	X_KEY_UNDERLINE =       0x93,   /*!< Underline key. (NEC PC98) */
	X_KEY_KANJI =           0x94,   /*!< Japanese Kanji key. */
	X_KEY_STOP =            0x95,   /*!< Stop key. (NEC PC98) */
	X_KEY_AX =              0x96,   /*!< Ax key. (Japan AX) */
	X_KEY_UNLABELED =       0x97,   /*!< Unlabeled. (J3100) */
	X_KEY_NEXTTRACK =       0x99,   /*!< Next track. */
	X_KEY_NUMPADENTER =     0x9C,   /*!< Enter on numeric keypad. */
	X_KEY_RCONTROL =        0x9D,	/*!< Right control key. */
	X_KEY_MUTE =            0xA0,   /*!< Mute key. */
	X_KEY_CALCULATOR =      0xA1,   /*!< Calculator */
	X_KEY_PLAYPAUSE =       0xA2,   /*!< Play / Pause key. */
	X_KEY_MEDIASTOP =       0xA4,   /*!< Media Stop key. */
	X_KEY_VOLUMEDOWN =      0xAE,   /*!< Volume - key. */
	X_KEY_VOLUMEUP =        0xB0,   /*!< Volume + key. */
	X_KEY_WEBHOME =         0xB2,   /*!< Web home key. */
	X_KEY_NUMPADCOMMA =     0xB3,   /*!< Comma (,) on numeric keypad. (NEC PC98) */
	X_KEY_DIVIDE =          0xB5,   /*!< Divide (/) on numeric keypad. */
	X_KEY_SYSRQ =           0xB7,	/*!< SysRQ key. */
	X_KEY_RMENU =           0xB8,   /*!< Right Alt key. */
	X_KEY_PAUSE =           0xC5,   /*!< Pause key. */
	X_KEY_HOME =            0xC7,   /*!< Home on arrow keypad. */
	X_KEY_UP =              0xC8,   /*!< Up arrow on arrow keypad. */
	X_KEY_PRIOR =           0xC9,   /*!< PgUp on arrow keypad. */
	X_KEY_LEFT =            0xCB,   /*!< Left arrow on arrow keypad. */
	X_KEY_RIGHT =           0xCD,   /*!< Right arrow on arrow keypad. */
	X_KEY_END =             0xCF,   /*!< End on arrow keypad. */
	X_KEY_DOWN =            0xD0,   /*!< Down arrow on arrow keypad. */
	X_KEY_NEXT =            0xD1,   /*!< PgDn on arrow keypad. */
	X_KEY_INSERT =          0xD2,   /*!< Insert on arrow keypad. */
	X_KEY_DELETE =          0xD3,   /*!< Delete on arrow keypad. */
	X_KEY_LWIN =            0xDB,   /*!< Left windows key. */
	X_KEY_RWIN =            0xDC,   /*!< Right windows key. */
	X_KEY_APPS =            0xDD,   /*!< AppMenu key. */
	X_KEY_POWER =           0xDE,   /*!< System power key. */
	X_KEY_SLEEP =           0xDF,   /*!< System sleep key. */
	X_KEY_WAKE =            0xE3,   /*!< System wake key. */
	X_KEY_WEBSEARCH =       0xE5,   /*!< Web search key.*/
	X_KEY_WEBFAVORITES =    0xE6,   /*!< Web favorites key.*/
	X_KEY_WEBREFRESH =      0xE7,   /*!< Web refresh key.*/
	X_KEY_WEBSTOP =         0xE8,   /*!< Web stop key.*/
	X_KEY_WEBFORWARD =      0xE9,   /*!< Web forward key.*/
	X_KEY_WEBBACK =         0xEA,   /*!< Web back key.*/
	X_KEY_MYCOMPUTER =      0xEB,   /*!< My Computer key.*/
	X_KEY_MAIL =            0xEC,   /*!< Mail key.*/
	X_KEY_MEDIASELECT =     0xED    /*!< Media select key.*/
};



//*****************************************************************************
//! Joystick/Gamepad button codes used for input driver routines.
/**
These defines specify joystick/gamepad button codes for game controllers.
*/
//*****************************************************************************
enum X_JOY_BUTTON
{
	X_JB_1,		/*!< Joystick/Gamepad button 1. */						 
	X_JB_2,		/*!< Joystick/Gamepad button 2. */									 
	X_JB_3,		/*!< Joystick/Gamepad button 3. */										
	X_JB_4,		/*!< Joystick/Gamepad button 4. */										 
	X_JB_5,		/*!< Joystick/Gamepad button 5. */										 
	X_JB_6,		/*!< Joystick/Gamepad button 6. */										 
	X_JB_7,		/*!< Joystick/Gamepad button 7. */										 
	X_JB_8,		/*!< Joystick/Gamepad button 8. */										 
	X_JB_9,		/*!< Joystick/Gamepad button 9. */										 
	X_JB_10,	/*!< Joystick/Gamepad button 10. */								 
	X_JB_11,	/*!< Joystick/Gamepad button 11. */									 
	X_JB_12,	/*!< Joystick/Gamepad button 12. */									 
	X_JB_13,	/*!< Joystick/Gamepad button 13. */									 
	X_JB_14,	/*!< Joystick/Gamepad button 14. */									 
	X_JB_15,	/*!< Joystick/Gamepad button 15. */									 
	X_JB_16,	/*!< Joystick/Gamepad button 16. */									 
	X_JB_17,	/*!< Joystick/Gamepad button 17. */								 
	X_JB_18,	/*!< Joystick/Gamepad button 18. */									 
	X_JB_19,	/*!< Joystick/Gamepad button 19. */									 
	X_JB_20,	/*!< Joystick/Gamepad button 20. */									 
	X_JB_21,	/*!< Joystick/Gamepad button 21. */								 
	X_JB_22,	/*!< Joystick/Gamepad button 22. */								 
	X_JB_23,	/*!< Joystick/Gamepad button 23. */								 
	X_JB_24,	/*!< Joystick/Gamepad button 24. */								 
	X_JB_25,	/*!< Joystick/Gamepad button 25. */								 
	X_JB_26,	/*!< Joystick/Gamepad button 26. */								 
	X_JB_27,	/*!< Joystick/Gamepad button 27. */								 
	X_JB_28,	/*!< Joystick/Gamepad button 28. */								 
	X_JB_29,	/*!< Joystick/Gamepad button 29. */								 
	X_JB_30,	/*!< Joystick/Gamepad button 30. */								 
	X_JB_31,	/*!< Joystick/Gamepad button 31. */							 
	X_JB_32		/*!< Joystick/Gamepad button 32. */	
};



#endif