/******************************************************************************/
/*!
\file TextureManager.h
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
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "TextureLoader.h"
#include <map>
#include <string>

typedef std::map<std::string, Texture> TextureContainer;

class TextureManager
{
public:
	//static inst
	static TextureManager* Inst();
	static void Destroy();

	void CreateTexture(const std::string & Path, const std::string &ObjName);
	Texture* GetTexture(const std::string& ObjName);

private:
	TextureManager();
	~TextureManager();
	//data
	TextureContainer mTextureContainer;
	static TextureManager* mInstance;
};

#define TextureManager_Inst TextureManager::Inst()




#endif

