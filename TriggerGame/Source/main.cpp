/******************************************************************************/
/*!
\file main.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for main function
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "mainGame.h"

int main(void)
{
	sGM = new MainGame;

#if defined(DEBUG) | defined(_DEBUG)
	sGM->PrintMemLeak(true);
#endif

	sGM->End();
	delete sGM;
	return 0;
}