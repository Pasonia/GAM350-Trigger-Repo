/******************************************************************************/
/*!
\file ObjectManager.h
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
#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "ObjectInstance.h"
#include <string>
#include <vector>

class ObjectManager
{
public:
	static ObjectManager& GetInstance();
	//void Load(const std::string& filename); //load from json
	ObjectInstance * CreateObjectInstance(const std::string& model_name, const std::string& texture_name);
	ObjectInstance * operator[](int i);
	void Update();
	void Render();
	void RenderShadow();
	void Free();
	void RemoveObject(ObjectInstance * obj);

	void SetDebugRender(int type);
	void UnsetDebugRender();

private:
	ObjectManager();
	ObjectManager(const ObjectManager&);
	void operator=(const ObjectManager&);

private:
	int flag;
	std::vector<ObjectInstance*> m_objects;
};

#define OBJECTMANAGER_INST ObjectManager::GetInstance()

#endif