/******************************************************************************/
/*!
\file TriggerInput.cpp
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#ifndef TRIGGER_INPUT_H_
#define TRIGGER_INPUT_H_

#include <windows.h>
#include "TriggerVector3D.h"
#include "TEMath.h"

#define OMVK_LBUTTON		 0x01	 /* Left mouse button */
#define OMVK_RBUTTON		 0x02	 /* Right mouse button */
#define OMVK_MBUTTON		 0x04	 /* Middle mouse button */

#define OMVK_BACK            0x08    /* backspace */
#define OMVK_TAB             0x09
#define OMVK_RETURN			 0x0D
#define OMVK_SHIFT			 0x10
#define OMVK_CONTROL		 0x11
#define OMVK_MENU			 0x12	 /* ALT key */
#define OMVK_PAUSE			 0x13
#define OMVK_CAPSLOCK		 0x14

#define OMVK_ESCAPE          0x1B

#define OMVK_SPACE			 0x20
#define OMVK_PRIOR			 0x21	 /* PAGE UP key */
#define OMVK_NEXT			 0x22	 /* PAGE DOWN key */
#define OMVK_END			 0x23
#define OMVK_H_OME			 0x24
#define OMVK_LEFT			 0x25
#define OMVK_UP				 0x26
#define OMVK_RIGHT			 0x27
#define OMVK_DOWN			 0x28
#define OMVK_INSERT			 0x2D
#define OMVK_DELETE			 0x2E

#define OMVK_0               0x30
#define OMVK_1               0x31
#define OMVK_2               0x32
#define OMVK_3               0x33
#define OMVK_4               0x34
#define OMVK_5               0x35
#define OMVK_6               0x36
#define OMVK_7               0x37
#define OMVK_8               0x38
#define OMVK_9               0x39

#define OMVK_A               0x41
#define OMVK_B               0x42
#define OMVK_C               0x43
#define OMVK_D               0x44
#define OMVK_E               0x45
#define OMVK_F               0x46
#define OMVK_G               0x47
#define OMVK_H_               0x48
#define OMVK_I               0x49
#define OMVK_J               0x4A
#define OMVK_K               0x4B
#define OMVK_L               0x4C
#define OMVK_M               0x4D
#define OMVK_N               0x4E
#define OMVK_O               0x4F
#define OMVK_P               0x50
#define OMVK_Q               0x51
#define OMVK_R               0x52
#define OMVK_S               0x53
#define OMVK_T               0x54
#define OMVK_U               0x55
#define OMVK_V               0x56
#define OMVK_W               0x57
#define OMVK_X               0x58
#define OMVK_Y               0x59
#define OMVK_Z               0x5A
#define OMVK_LWIN			 0x5B	 /* Left windows key */
#define OMVK_RWIN			 0x5C	 /* Right windows key */

#define OMVK_NUMPAD0		 0x60
#define OMVK_NUMPAD1		 0x61
#define OMVK_NUMPAD2		 0x62
#define OMVK_NUMPAD3		 0x63
#define OMVK_NUMPAD4		 0x64
#define OMVK_NUMPAD5		 0x65
#define OMVK_NUMPAD6		 0x66
#define OMVK_NUMPAD7		 0x67
#define OMVK_NUMPAD8		 0x68
#define OMVK_NUMPAD9		 0x69

#define OMVK_MULTIPLY		 0x6A
#define OMVK_ADD			 0x6B
#define OMVK_SUBTRACT		 0x6D
#define OMVK_DECIMAL		 0x6E
#define OMVK_DIVIDE			 0x6F

#define OMVK_F1				 0x70
#define OMVK_F2				 0x71
#define OMVK_F3				 0x72
#define OMVK_F4				 0x73
#define OMVK_F5				 0x74
#define OMVK_F6				 0x75
#define OMVK_F7				 0x76
#define OMVK_F8				 0x77
#define OMVK_F9				 0x78
#define OMVK_F10			 0x79
#define OMVK_F11			 0x7A
#define OMVK_F12			 0x7B

#define OMVK_NUMLOCK		 0x90
#define OMVK_SCROLL			 0x91

#define OMVK_LSHIFT          0xA0
#define OMVK_RSHIFT          0xA1
#define OMVK_LCONTROL        0xA2
#define OMVK_RCONTROL        0xA3

#define OMVK_COLON			 0xBA
#define OMVK_PLUS			 0xBB
#define OMVK_COMMA			 0xBC
#define OMVK_MINUS			 0xBD
#define OMVK_PERIOD			 0xBE
#define OMVK_QUESTION		 0xBF
#define OMVK_TILDE			 0xC0	 /* ~ tilde character */

#define OMVK_LBRACKET		 0xDB
#define OMVK_BACKSLASH		 0xDC
#define OMVK_RBRACKET		 0xDD
#define OMVK_QUOTE			 0xDE	 /* single-quote / double-quote */
#define key_pressed_down     0x80
#define key_pressed_up       0x81



class SYSTEMINPUT
{
public:

	/*static BYTE curr[256];
	static BYTE prev[256];
	HWND gHwnd;
	bool windowed = false;
	int  winHeight = 632;
	int  winWidth = 816;
	int  currHeight = 0;
	int  currWidth = 0;*/
	SYSTEMINPUT(){};
	~SYSTEMINPUT(){};


	void UpdateInput();

	// keyboard input
	bool TriggerKeyboardCheckCurr(unsigned char key);
	bool TriggerKeyboardCheckTriggered(unsigned char key);
	bool TriggerKeyboardCheckReleased(unsigned char key);
	bool TriggerKeyboardCheckDraggedX();
	bool TriggerKeyboardCheckDraggedY();
	void TriggerMouseInScreenSpace(Vec3 * mousepos);
	void TriggerMouseInClientSpace(Vec3 * mousepos, Vec3& WindowPos, Vec3& WindowSize);

	//show cursor
	void TriggerShowCursor(bool show = false);

	Vec3 clickedPos;

};

//// mouse input
//unsigned char OMMouseCheckCurr				(unsigned char key);
//unsigned char OMMouseCheckTriggered		(unsigned char key);
//unsigned char OMMouseCheckReleased         (unsigned char key);

#endif // OM_INPUT_H_
