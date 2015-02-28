/******************************************************************************/
/*!
\file ObjectMap.cpp
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

#include "ObjectMap.h"

namespace
{
	std::string empty = "";
}
//ObjectMap definitions
ObjectMap * ObjectMap::obj_map_instance = 0;

ObjectMap * ObjectMap::GetInstance()
{
	obj_map_instance = obj_map_instance ? obj_map_instance : new ObjectMap();
	return obj_map_instance;
}

void ObjectMap::AddObjectMap(const std::string& model_name, const Model * model)
{
	if (m_object_to_index_map.find(model_name) == m_object_to_index_map.end())
	{
		char index = m_object_map.size();
		m_object_map[index] = model_name;
		m_object_to_index_map[model_name] = index;
		m_model_to_index_map[model] = index;
	}
}
void ObjectMap::AddTextureMap(const std::string& texture_name)
{
	if (m_texture_to_index_map.find(texture_name) == m_texture_to_index_map.end())
	{
		char index = m_texture_map.size();
		m_texture_map[index] = texture_name;
		m_texture_to_index_map[texture_name] = index;
	}
}
const char ObjectMap::GetIndexToModel(const std::string& name)
{
	if (m_object_to_index_map.find(name) != m_object_to_index_map.end())
		return m_object_to_index_map[name];
	return -1;
}
const char ObjectMap::GetIndexToTexture(const std::string& name)
{
	if (m_texture_to_index_map.find(name) != m_texture_to_index_map.end())
		return m_texture_to_index_map[name];
	return -1;
}
const std::string& ObjectMap::GetMappedObjectName(char index)
{
	if (m_object_map.find(index) != m_object_map.end())
		return m_object_map[index];
	return empty;
}
const std::string& ObjectMap::GetMappedTextureName(char index)
{
	if (m_texture_map.find(index) != m_texture_map.end())
		return m_texture_map[index];
	return empty;
}
char ObjectMap::GetModelIndex(const Model * model)
{
	if (m_model_to_index_map.find(model) == m_model_to_index_map.end())
		return -1;
	return m_model_to_index_map[model];
}
ObjectMap::ObjectMap()
{

}
void ObjectMap::Clear()
{
	m_model_to_index_map.clear();
	m_object_map.clear();
	m_object_to_index_map.clear();
	m_texture_map.clear();
	m_texture_to_index_map.clear();
}

void ObjectMap::Destroy()
{
	if (obj_map_instance)
	{
		delete obj_map_instance;
		obj_map_instance = 0;
	}

}

//End ObjectMap definitions