/******************************************************************************/
/*!
\file   JSONWrapper.cpp
\author Long Jiquan Joel
\par	email: longjiquan.joel\@digipen.edu
\par	Digipen longjiquan.joel
\par    Course: GAM300
\par    Game Name: 
\date   03/9/2014
\brief  
    This file contains definitions for functions that read variables from
	JSON files.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "JSONWrapper.h"
#include <cstdio>
#include <typeinfo>
#include "Assert.h"
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

namespace Scripter{

	/*const char * OBJECT_TYPENAME[TYPE_TOTAL] = {typeid(bool).name(),
                                                typeid(int).name(),
                                                typeid(float).name(),
                                                typeid(double).name(),
                                                typeid(long).name(),
                                                typeid(char *).name()};*/
    
    void ExportDocument(Document * doc, const std::string& filename)
    {
      FILE * exp_file;
      fopen_s(&exp_file, filename.c_str(),"w");
      if (exp_file)
      {
    	  rapidjson::FileStream f(exp_file);
    	  rapidjson::PrettyWriter<rapidjson::FileStream> writer(f);
    	  doc->Accept(writer);	// Accept() traverses the DOM and generates Handler events.
      }
      fclose(exp_file);
    }
    
    //void (* FUNCTION_INIT[TOTAL_OBJECT_TYPES])(const Value&) = {NULL,NULL,...};
    
    /*void SetupObject(const Value& stuff)
    {
    }*/
}