/******************************************************************************/
/*!
\file FolderManager.cpp
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

#include "FolderManager.h"
#include "TriggerAssert.h"

int __super__dummy__ = 0; //resolve LNK4221

FolderManager * FolderManager::m_instance = 0;

FolderManager * FolderManager::GetInstance()
{
	m_instance = m_instance ? m_instance : new FolderManager();
	return m_instance;
}
void FolderManager::AddMap(const std::string& resource, const std::string& filepath)
{
	if (m_map.find(resource) == m_map.end())
		m_map[resource] = filepath;
}
const std::string& FolderManager::GetFolder(const std::string& resource)
{
	if (m_map.find(resource) == m_map.end())
		return "";
	return m_map[resource];
}
FolderManager::FolderManager()
{

}
FolderManager::~FolderManager()
{

}