/******************************************************************************/
/*!
\file TextureLoader.cpp
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

#include "TextureLoader.h"

TextureLoader* TextureLoader::mInstance = 0;

TextureLoader* TextureLoader::Inst()
{
	mInstance = mInstance ? mInstance : new TextureLoader();

	return mInstance;
}

void TextureLoader::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

bool TextureLoader::LoadTexture(const std::string FileName, Texture &mTexture)
{
	unsigned char *imageData;
	int imageWidth, imageHeight, numComponents;

	/* Read ball color texture */
	//"../Resource/Image/Face.png"
	if (ReadImageFile(FileName.c_str(), &imageData, &imageWidth, &imageHeight, &numComponents) == 0)
	{
		std::cout << "Failed to read png!\n";   // color texture not available ...
		return false;
	}
	
	GenerateTexture(&mTexture.mTextureID, imageData, imageWidth, imageHeight, numComponents);
	++mTexture.mID;

	free(imageData);


	return true;
}

void TextureLoader::GenerateTexture(GLuint *texID, unsigned char *imageData, int imageWidth, int imageHeight, int numComponents)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);
	//generate the image
	if (numComponents == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	/* ................................*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{

}
