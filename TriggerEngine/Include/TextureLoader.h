/******************************************************************************/
/*!
\file TextureLoader.h
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
#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include "Model.h"

class TextureLoader
{
public:
	static TextureLoader* Inst();
	static void Destroy();
	//load texture from png
	bool LoadTexture(const std::string FileName, Texture &mTexture);

private:
	void GenerateTexture(GLuint *texID, unsigned char *imageData, int imageWidth, int imageHeight, int numComponents);
	static TextureLoader* mInstance;
	TextureLoader();
	~TextureLoader();

};

#define TextureLoader_Inst TextureLoader::Inst()







#endif