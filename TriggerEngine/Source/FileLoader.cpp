/******************************************************************************/
/*!
\file   FileLoader.cpp
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
    This file contains file loading function definitions.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "FileLoader.h"
#include "WholeLine.h"

//Copy Constructor for ScriptInfo
ScriptInfo::ScriptInfo(const ScriptInfo& src):
	mID(src.mID),mParameters(src.mParameters)
{
}

//Conversion Constructor from std::string to construct ScriptInfo
ScriptInfo::ScriptInfo(const std::string& input_str)
{
	//find colon delimiter
	size_t finder = input_str.find(':');
	//if delimiter not found, the entire line is the ID
	if (finder == std::string::npos)
	{
		mID = input_str;
	}
	else
	{
		//If delimiter found, the entire string before the delimiter
		//is the ID
		mID = input_str.substr(0,finder);

		//if there are more parameters after the delimiter
		if (input_str.size() != finder+1)
		{
			//Begin search for parameters
			size_t old_pos = finder+2;
			do
			{
				//find space delimiter.
				finder = input_str.find(' ',old_pos);
				//if found, the string between the current space and the next space
				//is a new parameter.
				if (finder != std::string::npos)
				{
					mParameters.push_back(input_str.substr(old_pos,finder-old_pos));
					old_pos = finder + 1;
				}
				//if not found, the remainder of the string is the next parameter
				else
				{
					mParameters.push_back(input_str.substr(old_pos));
				}
			}while (finder != std::string::npos);
			//exit search if finder finds no more space delimiters.
		}
	}
}

Script LoadFileInfo(const std::string& filename)
{
	//initialize a script
	Script script;
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
			script.push_back(ScriptInfo(itr));
		}
	}
	//close input file
	input_file.close();
	return script;
}

void LoadLevelFile(int * mapData, const std::string& filename)
{
    //open specified file
	std::ifstream input_file(filename.c_str());
	//if file is open
    int * mover = mapData;
	if (input_file.is_open())
	{
        while (!input_file.eof())
        {
            input_file >> *(mover);
            mover++;
        }
    }
  input_file.close();
}

void WriteLevelFile(int *mapData, const std::string& filename, int width, int height)
{
    std::ofstream output_file(filename.c_str());
    if (output_file.is_open())
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; ++j)
            {
                output_file << *(mapData+j+(width*i)) << " ";
            }
            output_file << std::endl;
        }
    }
}