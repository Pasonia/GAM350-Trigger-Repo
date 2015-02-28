/******************************************************************************/
/*!
\file Font.h
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

#ifndef FONT_H_
#define FONT_H_

#include <string>
#include "Opengl.h"
#include "ShaderCompiler.h"
#include "TEMath.h"

class TextureMapFont
{
public:
	TextureMapFont();
	~TextureMapFont();
	void PrintText2D(const char * text, int PosX, int PosY, int width, int height);

private:
	void Init();

	GLuint Text2DTextureID;
	GLuint Text2DVertexBufferID;
	GLuint Text2DUVBufferID;
	GLuint Text2DShaderID;
	GLuint Text2DUniformID;
};


#endif

