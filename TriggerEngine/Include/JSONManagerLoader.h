/******************************************************************************/
/*!
\file JSONManagerLoader.h
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
#ifndef JSONMANAGERLOADER_H_
#define JSONMANAGERLOADER_H_

#include "JSONWrapper.h"

void LoadModelsFromJSON(const std::string& filename);
void LoadTexturesFromJSON(const std::string& filename);
void LoadUIElementsFromJSON(const std::string& filename);
void LoadObjectInstancesFromJSON(const std::string& filename);
void LoadAudiofilesFromJSON(const std::string& filename);
void LoadPathNamesFromJSON(const std::string& filename);

#endif