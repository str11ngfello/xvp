--*****************************************************************************
--*****************************************************************************
--     _  __         _    ______ 
--    | |/ /        | |  / / __ \
--    |   / ______  | | / / /_/ /
--   /   | /_____/  | |/ / ____/ 
--  /_/|_|          |___/_/       
--
--	Extensible Visualization Platform (X-VP) (c) 
--	Copyright 2000. All Rights Reserved.
-- 
--	This source is protected by copyright laws. Unauthorized reproduction or 
--	distribution of this source will result in severe civil and criminal
--	penalties and will be prosecuted to the maximum extent possible under law.
--	
--	Module Name:	XConstants.lua
--	Author:			Lynn Duke
--	Creation Date:	5-29-2005
--
--	Description:	
--
--*****************************************************************************
--*****************************************************************************


X_INVALID_HANDLE = 0


--enum XStatus
	X_OK = 0							-- Successful completion.  
	X_ERR_MEMORY_EXHAUSTED = 1			-- Memory was exhausted during operation. 
	X_ERR_INVALID_PARAMS = 2			-- Invalid function parameters were received. 
	X_ERR_NOT_SUPPORTED = 3				-- Operation not supported in current version. 
	X_ERR_REFERENCES_EXIST = 4			-- References still exist. 
	X_ERR_NO_SUCH_OBJECT = 5			-- Object could not be found. 
	X_ERR_OPERATION_FAILED = 6			-- The requested operation failed. 
	X_ERR_LIMIT_REACHED = 7				-- Reached a limit that stopped processing. 		
	X_ERR_NOT_RUNNING = 8				-- Object has been shutdown/closed/not started. 
	X_ERR_NOT_READY = 9					-- Object not ready for the requested operation. 
	X_ERR_NOT_INITIALIZED = 10			-- Object not initialized. 
	X_ERR_PARTIAL_FAILURE = 11			-- The function partially failed. 
	X_ERR_REDUNDANT_OPERATION = 12		-- The opeation has already been done. 	
	X_ERR_WRONG_NUMBER_OF_PARAMS = 13	-- Wrong number of parameters specified. 


--enum X_MOUSE_BUTTON
	X_MB_LEFT = 0
	X_MB_RIGHT = 1
	X_MB_MIDDLE = 2


--enum X_KEY
	X_KEY_ESCAPE =          1	-- Escape key. 
	X_KEY_1 =               2	-- Numerical 1 key. 
	X_KEY_2 =               3	-- Numerical 2 key. 
	X_KEY_3 =               4	-- Numerical 3 key. 
	X_KEY_4 =               5	-- Numerical 4 key. 
	X_KEY_5 =               6	-- Numerical 5 key. 
	X_KEY_6 =               7	-- Numerical 6 key. 
	X_KEY_7 =               8	-- Numerical 7 key. 
	X_KEY_8 =               9	-- Numerical 8 key. 
	X_KEY_9 =               10	-- Numerical 9 key. 
	X_KEY_0 =               11	-- Numerical 0 key. 
	X_KEY_MINUS =           12   -- Dash/Minus key. 
	X_KEY_EQUALS =          13	-- Escape key. 
	X_KEY_BACK =            15   -- Backspace key. 
	X_KEY_TAB =             16	-- Tab key. 
	X_KEY_Q =               16	-- Q/q key. 
	X_KEY_W =               17	-- W/w key. 
	X_KEY_E =               18	-- E/e key. 
	X_KEY_R =               19	-- R/r key. 
	X_KEY_T =               20	-- T/t key. 
	X_KEY_Y =               21	-- Y/y key. 
	X_KEY_U =               22	-- U/u key. 
	X_KEY_I =               23	-- I/i key. 
	X_KEY_O =               24	-- O/o key. 
	X_KEY_P =               25	-- P/p key. 
	X_KEY_LBRACKET =        26	-- Left square bracket key. 
	X_KEY_RBRACKET =        27	-- Right square bracket key. 
	X_KEY_RETURN =          28   -- Enter/Return key. 
	X_KEY_LCONTROL =        29	-- Left control key. 
	X_KEY_A =               30	-- A/a key. 
	X_KEY_S =               31	-- S/s key. 
	X_KEY_D =               32	-- D/d key. 
	X_KEY_F =               33	-- F/f key. 
	X_KEY_G =               34	-- G/g key. 
	X_KEY_H =               35	-- H/h key. 
	X_KEY_J =               36	-- J/j key. 
	X_KEY_K =               37	-- K/k key. 
	X_KEY_L =               38	-- L/l key. 
	X_KEY_SEMICOLON =       39	-- Semicolon key. 
	X_KEY_APOSTROPHE =      40	-- Apostrophe key. 
	X_KEY_GRAVE =           41   -- Accent grave key. 
	X_KEY_LSHIFT =          42	-- Left shift key. 
	X_KEY_BACKSLASH =       43	-- Backslash key. 
	X_KEY_Z =               44	-- Z/z key. 
	X_KEY_X =               45	-- X/x key. 
	X_KEY_C =               46	-- C/c key. 
	X_KEY_V =               47	-- V/v key. 
	X_KEY_B =               48	-- B/b key. 
	X_KEY_N =               49	-- N/n key. 
	X_KEY_M =               50	-- M/m key. 
	X_KEY_COMMA =           51	-- Comma key. 
	X_KEY_PERIOD =          52   -- Period key. 
	X_KEY_SLASH =           53   -- Forward slash. 
	X_KEY_RSHIFT =          54	-- Right shift key. 
	X_KEY_MULTIPLY =        55   -- Multiply (*) key on numeric keypad. 
	X_KEY_LMENU =           56   -- Left Alt key. 
	X_KEY_SPACE =           57	-- Spacebar key. 
	X_KEY_CAPITAL =         58	-- Caps lock key. 
	X_KEY_F1 =              59	-- F1 function key. 
	X_KEY_F2 =              60	-- F2 function key. 
	X_KEY_F3 =              61	-- F3 function key. 
	X_KEY_F4 =              62	-- F4 function key. 
	X_KEY_F5 =              63	-- F5 function key. 
	X_KEY_F6 =              64	-- F6 function key. 
	X_KEY_F7 =              65	-- F7 function key. 
	X_KEY_F8 =              66	-- F8 function key. 
	X_KEY_F9 =              67	-- F9 function key. 
	X_KEY_F10 =             68	-- F10 function key. 
	X_KEY_NUMLOCK =         69	-- Numlock key. 
	X_KEY_SCROLL =          70   -- Scroll Lock 
	X_KEY_NUMPAD7 =         71	-- Escape key. 
	X_KEY_NUMPAD8 =         72	-- Escape key. 
	X_KEY_NUMPAD9 =         73	-- Escape key. 
	X_KEY_SUBTRACT =        74   -- Subtract (-) on numeric keypad 
	X_KEY_NUMPAD4 =         75	-- Escape key. 
	X_KEY_NUMPAD5 =         76	-- Escape key. 
	X_KEY_NUMPAD6 =         77	-- Escape key. 
	X_KEY_ADD =             78   -- Add (+) on numeric keypad. 
	X_KEY_NUMPAD1 =         79	-- Numerical 1 on keypad. 
	X_KEY_NUMPAD2 =         80	-- Numerical 2 on keypad. 
	X_KEY_NUMPAD3 =         81	-- Numerical 3 on keypad. 
	X_KEY_NUMPAD0 =         82	-- Numerical 0 on keypad. 
	X_KEY_DECIMAL =         83   -- Decimal (.) on numeric keypad. 
	X_KEY_OEM_102 =         86   -- < > | on UK/Germany keyboards. 
	X_KEY_F11 =             87	-- F11 function key. 
	X_KEY_F12 =             88	-- F12 function key. 
	X_KEY_F13 =             100   -- F13 function key. (NEC PC98) 
	X_KEY_F14 =             101   -- F14 function key. (NEC PC98) 
	X_KEY_F15 =             102   -- F15 function key. (NEC PC98) 
	X_KEY_KANA =            112   -- (Japanese keyboard)            
	X_KEY_ABNT_C1 =         115   -- / ? on Portugese (Brazilian) keyboard. 
	X_KEY_CONVERT =         121   -- Convert key. (Japanese keyboard) 
	X_KEY_NOCONVERT =       123   -- (Japanese keyboard)  
	X_KEY_YEN =             125   -- (Japanese keyboard)            
	X_KEY_ABNT_C2 =         126   -- Numpad . on Portugese (Brazilian) keyboard 
	X_KEY_NUMPADEQUALS =    141   -- Equals (=) on numeric keypad. (NEC PC98) 
	X_KEY_PREVTRACK =       144   -- Previous Track. (X_KEY_CIRCUMFLEX on Japanese keyboard) 
	X_KEY_AT =              145   -- AT key. (NEC PC98) 
	X_KEY_COLON =           146   -- Colon key. (NEC PC98) 
	X_KEY_UNDERLINE =       147   -- Underline key. (NEC PC98) 
	X_KEY_KANJI =           148   -- Japanese Kanji key. 
	X_KEY_STOP =            149   -- Stop key. (NEC PC98) 
	X_KEY_AX =              150   -- Ax key. (Japan AX) 
	X_KEY_UNLABELED =       151   -- Unlabeled. (J3100) 
	X_KEY_NEXTTRACK =       153   -- Next track. 
	X_KEY_NUMPADENTER =     156   -- Enter on numeric keypad. 
	X_KEY_RCONTROL =        157	-- Right control key. 
	X_KEY_MUTE =            160   -- Mute key. 
	X_KEY_CALCULATOR =      161   -- Calculator 
	X_KEY_PLAYPAUSE =       162   -- Play / Pause key. 
	X_KEY_MEDIASTOP =       164   -- Media Stop key. 
	X_KEY_VOLUMEDOWN =      174   -- Volume - key. 
	X_KEY_VOLUMEUP =        176   -- Volume + key. 
	X_KEY_WEBHOME =         178   -- Web home key. 
	X_KEY_NUMPADCOMMA =     179   -- Comma (,) on numeric keypad. (NEC PC98) 
	X_KEY_DIVIDE =          181   -- Divide (/) on numeric keypad. 
	X_KEY_SYSRQ =           183	-- SysRQ key. 
	X_KEY_RMENU =           184   -- Right Alt key. 
	X_KEY_PAUSE =           197   -- Pause key. 
	X_KEY_HOME =            199   -- Home on arrow keypad. 
	X_KEY_UP =              200   -- Up arrow on arrow keypad. 
	X_KEY_PRIOR =           201   -- PgUp on arrow keypad. 
	X_KEY_LEFT =            203   -- Left arrow on arrow keypad. 
	X_KEY_RIGHT =           205   -- Right arrow on arrow keypad. 
	X_KEY_END =             207   -- End on arrow keypad. 
	X_KEY_DOWN =            208   -- Down arrow on arrow keypad. 
	X_KEY_NEXT =            209   -- PgDn on arrow keypad. 
	X_KEY_INSERT =          210   -- Insert on arrow keypad. 
	X_KEY_DELETE =          211   -- Delete on arrow keypad. 
	X_KEY_LWIN =            219   -- Left windows key. 
	X_KEY_RWIN =            220   -- Right windows key. 
	X_KEY_APPS =            221   -- AppMenu key. 
	X_KEY_POWER =           222   -- System power key. 
	X_KEY_SLEEP =           223   -- System sleep key. 
	X_KEY_WAKE =            227   -- System wake key. 
	X_KEY_WEBSEARCH =       229   -- Web search key.
	X_KEY_WEBFAVORITES =    230  -- Web favorites key.
	X_KEY_WEBREFRESH =      231  -- Web refresh key.
	X_KEY_WEBSTOP =         232  -- Web stop key.
	X_KEY_WEBFORWARD =      233   -- Web forward key.
	X_KEY_WEBBACK =         234   -- Web back key.
	X_KEY_MYCOMPUTER =      235   -- My Computer key.
	X_KEY_MAIL =            236   -- Mail key.
	X_KEY_MEDIASELECT =     237    -- Media select key.



--enum X_JOY_BUTTON
	X_JB_1 = 0		-- Joystick/Gamepad button 1. 						 
	X_JB_2 = 1		-- Joystick/Gamepad button 2. 									 
	X_JB_3 = 2		-- Joystick/Gamepad button 3. 										
	X_JB_4 = 3		-- Joystick/Gamepad button 4. 										 
	X_JB_5 = 4		-- Joystick/Gamepad button 5. 										 
	X_JB_6 = 5		-- Joystick/Gamepad button 6. 										 
	X_JB_7 = 6		-- Joystick/Gamepad button 7. 										 
	X_JB_8 = 7		-- Joystick/Gamepad button 8. 										 
	X_JB_9 = 8		-- Joystick/Gamepad button 9. 										 
	X_JB_10 = 9	-- Joystick/Gamepad button 10. 								 
	X_JB_11 = 10	-- Joystick/Gamepad button 11. 									 
	X_JB_12 = 11	-- Joystick/Gamepad button 12. 									 
	X_JB_13 = 12	-- Joystick/Gamepad button 13. 									 
	X_JB_14 = 13	-- Joystick/Gamepad button 14. 									 
	X_JB_15 = 14	-- Joystick/Gamepad button 15. 									 
	X_JB_16 = 15	-- Joystick/Gamepad button 16. 									 
	X_JB_17 = 16	-- Joystick/Gamepad button 17. 								 
	X_JB_18 = 17	-- Joystick/Gamepad button 18. 									 
	X_JB_19 = 18	-- Joystick/Gamepad button 19. 									 
	X_JB_20 = 19	-- Joystick/Gamepad button 20. 									 
	X_JB_21 = 20	-- Joystick/Gamepad button 21. 								 
	X_JB_22 = 21	-- Joystick/Gamepad button 22. 								 
	X_JB_23 = 22	-- Joystick/Gamepad button 23. 								 
	X_JB_24 = 23	-- Joystick/Gamepad button 24. 								 
	X_JB_25 = 24	-- Joystick/Gamepad button 25. 								 
	X_JB_26 = 25	-- Joystick/Gamepad button 26. 								 
	X_JB_27 = 26	-- Joystick/Gamepad button 27. 								 
	X_JB_28 = 27	-- Joystick/Gamepad button 28. 								 
	X_JB_29 = 28	-- Joystick/Gamepad button 29. 								 
	X_JB_30 = 29	-- Joystick/Gamepad button 30. 								 
	X_JB_31 = 30	-- Joystick/Gamepad button 31. 							 
	X_JB_32 = 31	-- Joystick/Gamepad button 32. 	


--enum X_UI_LUA_EVENTS
	X_UILE_ADDED_TO_SCENE = 0
	X_UILE_REM_FROM_SCENE = 1
	X_UILE_CHANGE = 2
	X_UILE_MOUSE_OVER = 3
	X_UILE_MOUSE_ENTER = 4
	X_UILE_MOUSE_LEAVE = 5

--enum X_TB_JUST_TYPE
	X_TBJ_NONE = 0
	X_TBJ_LEFT = 1
	X_TBJ_RIGHT = 2
	X_TBJ_CENTER = 3

--enum X_POINTER_STATE
	X_PS_NORMAL = 0
	X_PS_HELP = 1
	X_PS_BUSY = 2
	X_PS_WORKING_IN_BACKGROUND = 3
	X_PS_PRECISION_SELECT = 4
	X_PS_TEXT_SELECT = 5
	X_PS_UNAVAILABLE = 6
	X_PS_VERTICAL_RESIZE = 7
	X_PS_HORIZONTAL_RESIZE = 8
	X_PS_DIAGONAL_RESIZE_1 = 9
	X_PS_DIAGONAL_RESIZE_2 = 10
	X_PS_MOVE = 11
	X_PS_CUSTOM_1 = 12
	X_PS_CUSTOM_2 = 13
	X_PS_CUSTOM_3 = 14
	X_PS_CUSTOM_4 = 15
	X_PS_CUSTOM_5 = 16
	X_PS_CUSTOM_6 = 17
	X_PS_CUSTOM_7 = 18
	X_PS_CUSTOM_8 = 19
