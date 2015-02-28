/******************************************************************************/
/*!
\file FolderManager.h
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

#ifndef TRIGGERFOLDER_MANAGER_H_
#define TRIGGERFOLDER_MANAGER_H_

#include "JSONWrapper.h"
#include <string>
#include <map>

//typedef std::map<std::string, std::string> ResourceToFolderMap;

class FolderManager
{
public:
	static FolderManager * GetInstance();
	const std::string& GetFolder(const std::string& resource);
	void AddMap(const std::string& resource, const std::string& filepath);
private:
	static FolderManager * m_instance;
	std::map<std::string, std::string> m_map;
private:
	FolderManager();
	~FolderManager();
};

#define FOLDERMANAGER_INST FolderManager::GetInstance()

#endif 
