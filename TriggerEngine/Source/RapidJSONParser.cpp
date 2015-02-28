/******************************************************************************/
/*!
\file   RapidJSONParser.cpp
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
    This file contains custom definitions for parsing RapidJSON documents.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "TriggerAssert.h"
#include "FileLoader.h"
#include "WholeLine.h"

#include <cstdio> //for copying of buffer

using rapidjson::Document;

void GetJSONObject(JSONDOC * doc, const std::string& filename)
{
	std::string to_parse;
	//open specified file
	std::ifstream input_file(filename.c_str());
	//if file is open
	if (input_file.is_open())
	{
		//declare a vector that takes in file input
		std::vector<std::string> myinputs;
		//copy all lines from start to end of file
		std::istream_iterator<WholeLine> eoi;
		std::istream_iterator<WholeLine> input_file_stream(input_file);
    
		std::copy(input_file_stream,eoi,std::back_inserter(myinputs));

		//add each line taken from file, convert to scriptinfo, then push back.
		for (auto const& itr: myinputs)
		{
			to_parse.append(itr);
		}

		doc->Parse<0>(to_parse.c_str());

    ASSERT(!(doc->Parse<0>(to_parse.c_str()).HasParseError()),"JSONScript Parse Error!");
	}
  input_file.close();
}