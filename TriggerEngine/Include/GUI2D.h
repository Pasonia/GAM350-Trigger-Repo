/******************************************************************************/
/*!
\file GUI2D.h
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
#ifndef GUI2D_H_
#define GUI2D_H_

#include <string>
#include "Opengl.h"
#include "ShaderCompiler.h"
#include "TEMath.h"
#include <map>

#define GUI2DSIZE 1

struct ImageGUI2D
{
	ImageGUI2D() :Image2DTextureID(-1), ROW(1), COL(1),Start(1),End(1){}
	GLuint Image2DTextureID;
	unsigned int ROW, COL;
	unsigned int Start, End;
};

class IMAGE2D
{
public:
	static IMAGE2D* Inst();
	void Load2DGUIImage(const std::string &path,const std::string &name, unsigned int ROW, unsigned int COL, unsigned Start, unsigned End);
	void Render(const std::string& name, const Vec2& Pos, const Vec2& Scale, float dt, float alpha = 0.f, bool isAlpha = false, bool isLoop = false);
	static void Destroy();

private:
	IMAGE2D();
	~IMAGE2D();
	static IMAGE2D* mInstance;
	GLuint Image2DVertexBufferID;
	GLuint Image2DUVBufferID;
	GLuint Image2DShaderID;
	GLuint Image2DUniformID;
	GLuint Image2DAlphaID;
	GLuint Image2DisAlpha;
	std::map<std::string, ImageGUI2D> Image2DContainer;
};

#define IMAGE2D_Inst IMAGE2D::Inst()

#endif





