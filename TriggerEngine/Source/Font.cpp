/******************************************************************************/
/*!
\file Font.cpp
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

#include "Font.h"
#include "model.h"
#include "texturemanager.h"
#include "system.h"

TextureMapFont::TextureMapFont()
{
	Init();
}

TextureMapFont::~TextureMapFont()
{
	glDeleteProgram(Text2DShaderID);
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);
}

void TextureMapFont::Init()
{
	//get texture
	Texture t_tex;
	TextureLoader_Inst->LoadTexture("../Resource/Image/dsdigitalfonts.png", t_tex);
	Text2DTextureID = t_tex.mTextureID;

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	Text2DShaderID = CompileShaders("../Resource/Shader/VertShader/2DFont_vs.shader", "../Resource/Shader/FragShader/2DFont_fs.shader");
	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");
}

char CheckCharacterX(const char c)
{
	if (c >= '0' && c <= '9') // it number
		return c - '0';
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 21;
	else if (c >= 'A' && c <= 'O')
		return c - 'A' + 1;
	else if (c >= 'P' && c <= 'Z')
		return c - 'P';
	else if (c == '.')
		return 14;
	else if (c == ':')
		return 10;
	else
		return 0;
}
char CheckCharacterY(const char c)
{
	if (c >= '0' && c <= '9') // it number
		return 4;
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 21;
	else if (c >= 'A' && c <= 'O')
		return 1;
	else if (c >= 'P' && c <= 'Z')
		return 0;
	else if (c == '.')
		return 5;
	else if (c == ':')
		return 4;
	else
		return 5;
}

void TextureMapFont::PrintText2D(const char *text, int PosX, int PosY, int width, int height)
{
	unsigned int length = strlen(text);
	// Fill buffers
	std::vector<Vec2> vertices;
	std::vector<Vec2> UVs;
	for (unsigned int i = 0; i<length; i++)
	{
		/*
		glm::vec2 vertex_up_left = glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i * size, y);
		*/
		Vec2 vertex_up_left    = Vec2(PosX + i * width, PosY + height);
		Vec2 vertex_up_right   = Vec2(PosX + i * width + height, PosY + height);
		Vec2 vertex_down_right = Vec2(PosX + i * width + height, PosY);
		Vec2 vertex_down_left  = Vec2(PosX + i * width, PosY);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_down_right);

		//char character =  - '0' + 10;
		char characterX = CheckCharacterX(text[i]);
		char characterY = CheckCharacterY(text[i]);
		float uv_x = (characterX % 16) / 16.0f;
		float uv_y = (characterY % 8) /  8.0f;
		float dx = 1.0f / 16.f;
		float dy = 1.f / 8.f;

		Vec2 uv_down_left  = Vec2(uv_x, uv_y); // 0 0 
		Vec2 uv_down_right = Vec2(uv_x + dx, uv_y); // 1 , 0
		Vec2 uv_up_right = Vec2(uv_x + dx, uv_y + dy); // 1, 1
		Vec2 uv_up_left = Vec2(uv_x, uv_y + dy); // 0, 1

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_down_right);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Text2DShaderID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//set window size
	System::GetInstance()->SetWindowSizeToShader(Text2DShaderID);
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}