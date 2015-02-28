/******************************************************************************/
/*!
\file Opengl.h
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
#ifndef _OPENGLCLASS_H_
#define _OPENGLCLASS_H_

// Include standard headers
#include <iostream>
#include <string>
#include <windows.h>
// Include GLEW
#include <glew.h>
// Include GLFW
#include <glfw3.h>

#include "TEMath.h"

class OpenGL
{
public:
	OpenGL();
	~OpenGL();
	bool InitOpenGL(GLFWwindow* Window, unsigned int height, unsigned int width, float Near, float Far);
	void SwapBuffer();
	void ClearBuffer();
	Mat4& GetPerspective();

private:
	void CreatePerspective(unsigned int height, unsigned int width, float Near, float Far);
	GLFWwindow* mw;
	float FOVY;
	Mat4 mPerspectiveMatrix;
};






#endif

