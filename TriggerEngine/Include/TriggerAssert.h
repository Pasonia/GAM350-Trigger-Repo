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
#ifndef TRIGGER_ASSERT_H_
#define TRIGGER_ASSERT_H_

#include <cstdarg>
#include <Windows.h>
#include <cstdio>
//Asserts
//Max character size of message box
const unsigned int MESSAGEMAXSIZE = 1023;

//Types of message box
enum MessageType
{
	MSG_ERROR,
	MSG_WARNING,
	MSG_PLAIN,

	MSG_MAX,
};

//Create a custom message box
void A_MessageBox(MessageType messageType,
	const char* formatedString,
	...);

#define ASSERT(x, ...)                                                               \
{                                                                                       \
if ((x) == 0)                                                                        \
{                                                                                   \
	char buffer[255];                                                                 \
	sprintf_s(buffer, __VA_ARGS__);                                                 \
	A_MessageBox(MSG_ERROR, \
	"ASSERT\nTest: %s\nLine: %d\nFunc: %s\nFile: %s\n\n%s\n", \
#x, __LINE__, __FUNCTION__, __FILE__, buffer);                    \
	exit(1);                                                                        \
}                                                                                   \
}

//Assert without test
#define ASSERT_MSG(...)                                                              \
{                                                                                       \
	char buffer[255];                                                                     \
	sprintf_s(buffer, __VA_ARGS__);                                                     \
	A_MessageBox(MSG_ERROR, \
	"ASSERT_MSG\nLine: %d\nFunc: %s\nFile: %s\n\n%s\n", \
	__LINE__, __FUNCTION__, __FILE__, buffer);                            \
	exit(1);                                                                            \
}

//Warning with test
#define WARNING(x, ...)                                                              \
{                                                                                       \
if ((x) == 0)                                                                        \
{                                                                                   \
	char buffer[255];                                                                 \
	sprintf_s(buffer, __VA_ARGS__);                                                 \
	A_MessageBox(MSG_WARNING, \
	"WARNING\nTest: %s\nLine: %d\nFunc: %s\nFile: %s\n\n%s\n", \
#x, __LINE__, __FUNCTION__, __FILE__, buffer);                    \
}                                                                                   \
}

//Warning without test
#define WARNING_MSG(...)                                                             \
{                                                                                       \
	char buffer[255];                                                                     \
	sprintf_s(buffer, __VA_ARGS__);                                                     \
	A_MessageBox(MSG_WARNING, \
	"WARNING_MSG\nLine: %d\nFunc: %s\nFile: %s\n\n%s\n", \
	__LINE__, __FUNCTION__, __FILE__, buffer);                            \
}

#endif