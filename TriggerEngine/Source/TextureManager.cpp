/******************************************************************************/
/*!
\file TextureManager.cpp
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

#include "TextureManager.h"

TextureManager* TextureManager::mInstance = 0;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for (auto it = mTextureContainer.begin(); it != mTextureContainer.end(); ++it)
		glDeleteTextures(1, &it->second.mTextureID);

	mTextureContainer.clear();
}

void TextureManager::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

TextureManager* TextureManager::Inst()
{
	mInstance = mInstance ? mInstance : new TextureManager();
	return mInstance;
}

void TextureManager::CreateTexture(const std::string & Path, const std::string &ObjName)
{
	if (GetTexture(ObjName))
		return;
	Texture mTexture;
	if (!TextureLoader_Inst->LoadTexture(Path, mTexture))
	{
		//assert fail
	}
	mTextureContainer.insert(std::make_pair(ObjName, mTexture));
}

Texture* TextureManager::GetTexture(const std::string& ObjName)
{
	auto itr = mTextureContainer.find(ObjName);
	if (itr != mTextureContainer.end())
	{
		return &(itr->second);
	}
	else
	{
		return NULL;
	}
}
