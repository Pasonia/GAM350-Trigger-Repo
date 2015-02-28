/******************************************************************************/
/*!
\file   FileLoader.h
\author Long Jiquan Joel
\par    email: guangtian.guo\@digipen.edu
\par	email: julian.teh\@digipen.edu
\par	email: shikang.n\@digipen.edu
\par	email: longjiquan.joel\@digipen.edu
\par    DigiPen guangtian.guo
\par	DigiPen julian.teh
\par	Digipen shikang.n
\par	Digipen longjiquan.joel
\par    Course: GAM200
\par    Game Name: TeleK
\date   01/8/2013
\brief  
    This file contains declarations for FileLoader objects.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef FILELOADER_H_
#define FILELOADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//RapidJSON includes.
#include "rapidjson/document.h"		// rapidjson's DOM-style API
typedef rapidjson::Document JSONDOC;

//ScriptInfo
class ScriptInfo
{
private:
	std::string mID;
	std::vector<std::string> mParameters;
public:
	ScriptInfo(){};
	ScriptInfo(const ScriptInfo& src);
	ScriptInfo(const std::string& input_str);

	const std::string& GetID() const{return mID;}
	const std::vector<std::string>& GetParameters() const
	{
		return mParameters;
	};
};


typedef std::vector<ScriptInfo> Script;

void LoadLevelFile(int * mapData, const std::string& filename);
void WriteLevelFile(int *mapData, const std::string& filename, int width, int height);

//Functions

//Load a Script.
Script LoadFileInfo(const std::string& filename);
//Parse a JSON Script.
void GetJSONObject(JSONDOC * doc, const std::string& filename);

#endif

