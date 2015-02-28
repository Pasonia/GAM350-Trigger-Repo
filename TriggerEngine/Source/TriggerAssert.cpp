/******************************************************************************/
/*!
\file TriggerAssert.cpp
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

#include "TriggerAssert.h"
#define _CRT_SECURE_NO_WARNINGS
/******************************************************************************
Message box
******************************************************************************/
//Create a custom message box
void A_MessageBox(MessageType messageType, const char* formatedString, ...)
{
	//temp buffer to store the message
	char textBuffer[MESSAGEMAXSIZE + 1];

	//Create a variable arugment list
	// and set it to the formated string
	va_list argList;
	va_start(argList, formatedString);

	//Read the formated string into the buffer
	// and set the last character as a null-terminator
	vsnprintf_s(textBuffer, MESSAGEMAXSIZE, formatedString, argList);
	textBuffer[MESSAGEMAXSIZE] = '\0';

	//Depending on the message type,
	// display different message box
	switch (messageType)
	{
	case MSG_ERROR:
	{
					  //Display the message box with an error icon
				//	  MessageBox(NULL, textBuffer, "Error", MB_ICONERROR | MB_OK);
					  break;
	}

	case MSG_WARNING:
	{
						//Display the message box with an warning icon
					//	MessageBox(NULL, textBuffer, "Warning", MB_ICONWARNING | MB_OK);
						break;
	}

	case MSG_PLAIN:
	{
					  //Display the message box with no icon
					//  MessageBox(NULL, textBuffer, "Message", MB_OK);
					  break;
	}

	default: //Invalid or unhandled message type
	{
				 //Give a warning to the user
				 //WARNING_MSG("Unhandled MessageType: %i", messageType);

				 //Display as plain messagebox
				 //MessageBox(NULL, textBuffer, "Message", MB_OK);
				 break;
	}

	} //end switch (messageType)
}