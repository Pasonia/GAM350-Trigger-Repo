/******************************************************************************/
/*!
\file GUI2D.cpp
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

#include "GUI2D.h"
#include "model.h"
#include "texturemanager.h"
#include "system.h"

IMAGE2D* IMAGE2D::mInstance = 0;

IMAGE2D* IMAGE2D::Inst()
{
	mInstance = mInstance ? mInstance : new IMAGE2D();
	return mInstance;
}

IMAGE2D::IMAGE2D()
{
	// Initialize VBO
	glGenBuffers(1, &Image2DVertexBufferID);
	glGenBuffers(1, &Image2DUVBufferID);

	// Initialize Shader
	Image2DShaderID = CompileShaders("../Resource/Shader/VertShader/2DGUI_vs.shader", "../Resource/Shader/FragShader/2DGUI_fs.shader");
	// Initialize uniforms' IDs
	Image2DUniformID = glGetUniformLocation(Image2DShaderID,"ImageGUI2D");

	Image2DAlphaID = glGetUniformLocation(Image2DShaderID, "alpha_value");

	Image2DisAlpha = glGetUniformLocation(Image2DShaderID, "isAlpha");
}

IMAGE2D::~IMAGE2D()
{
	for (auto it = Image2DContainer.begin(); it != Image2DContainer.end(); ++it)
		glDeleteTextures(1, &it->second.Image2DTextureID);

	Image2DContainer.clear();

	glDeleteProgram(Image2DShaderID);
	glDeleteBuffers(1, &Image2DVertexBufferID);
	glDeleteBuffers(1, &Image2DUVBufferID);
}

void IMAGE2D::Load2DGUIImage(const std::string &path, const std::string &name, unsigned int ROW, unsigned int COL, unsigned Start, unsigned End)
{
	auto itr = Image2DContainer.find(name);
	if (itr != Image2DContainer.end())
		return;
	//get texture
	Texture t_tex;
	TextureLoader_Inst->LoadTexture(path.c_str(), t_tex);
	ImageGUI2D tempImagedata;
	tempImagedata.Image2DTextureID = t_tex.mTextureID;
	tempImagedata.ROW = ROW;
	tempImagedata.COL = COL;
	tempImagedata.Start = Start;
	tempImagedata.End = End;
	Image2DContainer.insert(std::make_pair(name, tempImagedata));
}

void IMAGE2D::Render(const std::string& name, const Vec2& Pos,const Vec2& Scale, float dt, float alpha, bool isAlpha, bool isLoop)
{
	auto itr = Image2DContainer.find(name);
	ASSERT(itr != Image2DContainer.end(), "GUI FILE NAME ERROR");
	//init UV and vertices of image
	std::vector<Vec2> vertices;
	std::vector<Vec2> UVs;
	Vec2 vertex_up_left = Vec2(Pos.x, Pos.y + GUI2DSIZE * Scale.y);
	Vec2 vertex_up_right = Vec2(Pos.x + GUI2DSIZE * Scale.x, Pos.y + GUI2DSIZE * Scale.y);
	Vec2 vertex_down_right = Vec2(Pos.x + GUI2DSIZE * Scale.x, Pos.y);
	Vec2 vertex_down_left = Vec2(Pos.x, Pos.y);

	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_right);
	vertices.push_back(vertex_up_right);

	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_left);
	vertices.push_back(vertex_down_right);

	unsigned int start = itr->second.Start;
	unsigned int end = itr->second.End;
	unsigned int row = itr->second.ROW;
	unsigned int col = itr->second.COL;

	float x_len = 1.f / col;
	float y_len = 1.f / row;

	float uv_x = x_len * (start % col);
	float uv_y = 1.f - (y_len * (start / col));

	Vec2 uv_down_left	= Vec2(uv_x			, uv_y - y_len	); // 0 0 
	Vec2 uv_down_right	= Vec2(uv_x + x_len	, uv_y - y_len	); //1, 0
	Vec2 uv_up_left		= Vec2(uv_x			, uv_y			); // 0, 1
	Vec2 uv_up_right	= Vec2(uv_x + x_len	, uv_y			); //1, 1

	UVs.push_back(uv_up_left);
	UVs.push_back(uv_down_right);
	UVs.push_back(uv_up_right);

	UVs.push_back(uv_up_left);
	UVs.push_back(uv_down_left);
	UVs.push_back(uv_down_right);

	glBindBuffer(GL_ARRAY_BUFFER, Image2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Image2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Image2DShaderID);
	//set window size
	System::GetInstance()->SetWindowSizeToShader(Image2DShaderID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glUniform1f(Image2DAlphaID, alpha);
	glUniform1i(Image2DisAlpha, isAlpha);
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, itr->second.Image2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Image2DUniformID, 0);
	
	
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Image2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Image2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void IMAGE2D::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}



