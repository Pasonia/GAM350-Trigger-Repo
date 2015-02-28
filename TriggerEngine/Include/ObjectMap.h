/******************************************************************************/
/*!
\file ObjectMap.h
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
#ifndef OBJECTMAP_H_
#define OBJECTMAP_H_

#include <string>
#include <map>

class Model;

class ObjectMap
{
public:
	static ObjectMap * GetInstance();
	void AddObjectMap(const std::string& model_name, const Model * model);
	void AddTextureMap(const std::string& texture_name);
	const char GetIndexToModel(const std::string& name);
	const char GetIndexToTexture(const std::string& name);
	const std::string& GetMappedObjectName(char index);
	const std::string& GetMappedTextureName(char index);
	char GetModelIndex(const Model * model);
	int GetObjectMapSize(void) { return m_object_map.size(); }
	int GetTextureMapSize(void) { return m_texture_map.size(); }
	void Clear();
	static void Destroy();
private:
	static ObjectMap * obj_map_instance;
	ObjectMap();
	ObjectMap(const ObjectMap&);
	void operator=(const ObjectMap&);
	std::map < char, std::string > m_object_map;
	std::map < char, std::string > m_texture_map;
	std::map < std::string, char > m_object_to_index_map;
	std::map < std::string, char > m_texture_to_index_map;
	std::map < const Model *, char > m_model_to_index_map;
};

#define OBJMAP_INST ObjectMap::GetInstance()

#endif