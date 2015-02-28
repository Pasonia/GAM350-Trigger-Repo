/******************************************************************************/
/*!
\file   JSONWrapper.h
\author Long Jiquan Joel
\author Long Jiquan Joel
\par	email: longjiquan.joel\@digipen.edu
\par	Digipen longjiquan.joel
\par    Course: GAM300
\par    Game Name: 
\date   03/9/2014
\brief  
    This file contains declarations for functions that read variables from
	JSON files.

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef FILE_VARIABLE_READER_H_
#define FILE_VARIABLE_READER_H_

#include "FileLoader.h"
#include <list>

struct EmitterData;
namespace Scripter
{
  typedef rapidjson::Document Document;
  typedef rapidjson::Value Value;

  void InitializeDocument(Document * doc);
  void InitializeObject(Value & val);
  void InitializeArray(Value& val);

  void ExportDocument(Document * doc, const std::string& filename);

  template<typename T>
  void Pushback_To_Array(rapidjson::Value& Val,T obj, 
                          const char * string, Document * doc);
  template<typename T>
  void AppendToJSONObject(Value& obj, const T& variable,
                          const char * string, Document * doc);

  void AppendArrayToJSONObject(Value& src_obj, Value& dest_obj, const char * string,
  Document * doc);
  void AppendJSONObjectToJSONObject(Value& src_obj, Value& dest_obj, const char * name, Document * doc);

  void AppendStringToJSONObject(Value& obj, const char * tag, const char * str, Document * doc);
  //Extract Values from rapidjson::Values
  void GetInt(Document * doc, char * var_name, int * dest);
  void GetDouble(Document * doc, char * var_name, double * dest);/*
  void GetInt(const Value * doc, char * var_name, int * dest);
  void GetDouble(const Value * doc, char * var_name, double * dest);*/
}

#include "JSONWrapper.inl"

#endif