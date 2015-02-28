/******************************************************************************/
/*!
\file Opengl.cpp
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

#include "Opengl.h"

OpenGL::OpenGL() :FOVY(PI_QUARTER)
{
}


OpenGL::~OpenGL()
{

}

bool OpenGL::InitOpenGL(GLFWwindow* Window, unsigned int height, unsigned int width, float Near, float Far)
{
	//get thw GLFW window
	mw = Window;
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//create Perspective
	CreatePerspective(height,width,Near, Far);

	return true;
}

void OpenGL::SwapBuffer()
{
	// Swap buffers
	glfwSwapBuffers(mw);
	glfwPollEvents();
}

void OpenGL::ClearBuffer()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::CreatePerspective(unsigned int height, unsigned int width, float Near, float Far)
{
	float aspect = (float)16 / (float)9;
	//glViewport(0, 0, width, height);
	mPerspectiveMatrix = glm::perspective(FOVY, aspect, Near, Far);
}

Mat4& OpenGL::GetPerspective()
{
	return mPerspectiveMatrix;
}

