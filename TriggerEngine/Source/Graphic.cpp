/******************************************************************************/
/*!
\file Graphic.cpp
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

#include "Graphic.h"

#ifdef _WIN32
#pragma warning (disable : 4996)
#endif

static const GLfloat float_zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
static const GLfloat float_ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLenum  draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

Graphic::Graphic():isLightON_OFF(true),
				   isMotionBlurr(false)
{
}

Graphic::~Graphic()
{
	glDeleteProgram(FirstPass);
	glDeleteProgram(SecondPass);
	glDeleteProgram(SM_Light);
	//vao
	glDeleteVertexArrays(1, &fs_quad_vao);
	//delete camera
	if (mCamera)
		delete mCamera;
	//delete light
	if (mLighting)
		delete mLighting;

	SkyBoxInst->Destroy();
}

void Graphic::CreateCamera()
{
	mCamera = new Camera;

	mCamera->Init();
}

void Graphic::CreateBlinPhong()
{
	mLighting = new LightBlin(SecondPass);
}

void Graphic::DeferredRenderStart()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer);
	glDrawBuffers(3, draw_buffers);
	glClearBufferfv(GL_COLOR, 0, float_zeros);
	glClearBufferfv(GL_COLOR, 1, float_zeros);
	glClearBufferfv(GL_COLOR, 2, float_zeros);
	glClearBufferfv(GL_DEPTH, 0, float_ones);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	//first pass
	glUseProgram(FirstPass);
	
	/* Pass the viewing matrix into the shaders */
	glUniformMatrix4fv(deferred_viewLocation, 1, GL_FALSE, glm::value_ptr(mCamera->GetViewMatrix()));
	/* Pass the projection matrix into the shaders */
	glUniformMatrix4fv(deferred_projLocation, 1, GL_FALSE, glm::value_ptr(mPerspMtx));
}

void Graphic::DeferredRenderEnd()
{	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer);
	//glViewport(0, 0, width, height);
	//glDrawBuffer(GL_BACK);
}

void Graphic::Rendering()
{
	//second pass
	glUseProgram(SecondPass);
	//the 3 variables
	//color
	//postion
	//normal
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[2]);

	glDisable(GL_DEPTH_TEST);
	//lights update
	mLighting->LightBlinUpdate(mCamera->GetEye(), isLightON_OFF, isMotionBlurr);

	//draw to the quad
	glBindVertexArray(fs_quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//i = glGetError();
	//with texture from the quad
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

bool Graphic::DeferredStartUp(OpenGL *OpenGL_, unsigned int height, unsigned int width)
{
	//save the ptr of OpenGL
	mOpenGL = OpenGL_;
	//create framebuffers
	glGenFramebuffers(1, &gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
	//get the 4 buffer up
	//color postion normal and lastly depth
	glGenTextures(MAX_NUM_BUFFER, gbuffer_tex);
	// Generate and bind the texture for diffuse
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Generate and bind the texture for positions
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[1]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Generate and bind the texture for normals
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[2]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Bind the depth buffer
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[3]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, width, height);
	// Attach the texture to the FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gbuffer_tex[0], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, gbuffer_tex[1], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, gbuffer_tex[2], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  gbuffer_tex[3], 0);
	//final bind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//check if we create successfully
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Deferred Init: FrameBuffer incomplete FAIL " << std::endl;
		return false;
	}
	//create the [-1 , 1] quad
	glGenVertexArrays(1, &fs_quad_vao);
	glBindVertexArray(fs_quad_vao);
	//load the shader
	//FirstPass  = CompileShaders("../Resource/Shader/VertShader/Deferred_FirstPass_vs.shader", "../Resource/Shader/FragShader/Deferred_FirstPass_fs.shader");
	TriggerShader FirstVert, FirstFrag;
	FirstVert.LoadShader("../Resource/Shader/VertShader/Deferred_FirstPass_vs.shader", GL_VERTEX_SHADER);
	FirstFrag.LoadShader("../Resource/Shader/FragShader/Deferred_FirstPass_fs.shader", GL_FRAGMENT_SHADER);

	FirstPassProg.CreateProgram();
	FirstPassProg.AddShaderToProgram(&FirstVert);
	FirstPassProg.AddShaderToProgram(&FirstFrag);
	FirstPassProg.LinkProgram();

	//SecondPass = CompileShaders("../Resource/Shader/VertShader/Deferred_SecondPass_vs.shader", "../Resource/Shader/FragShader/Deferred_SecondPass_fs.shader");
	TriggerShader SecondVert, SecondFrag, SecondFragLight;
	SecondVert.LoadShader("../Resource/Shader/VertShader/Deferred_SecondPass_vs.shader", GL_VERTEX_SHADER);
	SecondFragLight.LoadShader("../Resource/Shader/FragShader/Deferred_SecondPass_fs_light.shader", GL_FRAGMENT_SHADER);
	SecondFrag.LoadShader("../Resource/Shader/FragShader/Deferred_SecondPass_fs.shader", GL_FRAGMENT_SHADER);

	SecondPassProg.CreateProgram();
	SecondPassProg.AddShaderToProgram(&SecondVert);
	SecondPassProg.AddShaderToProgram(&SecondFragLight);
	SecondPassProg.AddShaderToProgram(&SecondFrag);
	SecondPassProg.LinkProgram();

	//shadow map
	//SM_Light = CompileShaders("../Resource/Shader/VertShader/SM_Light_vert.shader", "../Resource/Shader/FragShader/SM_Light_frag.shader");
	TriggerShader ShadowVert, ShadowFrag;
	ShadowVert.LoadShader("../Resource/Shader/VertShader/SM_Light_vert.shader", GL_VERTEX_SHADER);
	ShadowFrag.LoadShader("../Resource/Shader/FragShader/SM_Light_frag.shader", GL_FRAGMENT_SHADER);

	ShadowLightProg.CreateProgram();
	ShadowLightProg.AddShaderToProgram(&ShadowVert);
	ShadowLightProg.AddShaderToProgram(&ShadowFrag);
	ShadowLightProg.LinkProgram();

	//forward rendering
	TriggerShader ForwardVert, ForwardFrag;
	ForwardVert.LoadShader("../Resource/Shader/VertShader/Forward_vert.shader", GL_VERTEX_SHADER);
	ForwardFrag.LoadShader("../Resource/Shader/FragShader/Forward_frag.shader", GL_FRAGMENT_SHADER);

	ForwardRenderProg.CreateProgram();
	ForwardRenderProg.AddShaderToProgram(&ForwardVert);
	ForwardRenderProg.AddShaderToProgram(&ForwardFrag);
	ForwardRenderProg.LinkProgram();

	FirstPass = FirstPassProg.GetProgramID();
	SecondPass = SecondPassProg.GetProgramID();
	SM_Light = ShadowLightProg.GetProgramID();

	ShadowMapModel = glGetUniformLocation(SM_Light, "model");
	ShadowMapProj  = glGetUniformLocation(SM_Light, "Lproj");
	ShadowMapView  = glGetUniformLocation(SM_Light, "Lview");
	//the matrix for deferred matrix
	deferred_viewLocation = glGetUniformLocation(FirstPass, "viewMat");
	deferred_projLocation = glGetUniformLocation(FirstPass, "projMat");
	//set perp and view
	mPerspMtx = mOpenGL->GetPerspective();
	//create camera
	CreateCamera();
	//create Light
	CreateBlinPhong();
	//create shadowmap
	CreateShadowBuffer(width,height);
	return true;
}

void Graphic::CreateShadowBuffer(unsigned int width, unsigned int height)
{
	// Create the FBO
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	// Create the depth buffer
	glGenTextures(1, &depth_tex);
	glBindTexture(GL_TEXTURE_2D, depth_tex);
	//create the the storage
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_tex, 0);

	// Disable writes to the color buffer, No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Create Depth texture fail" << std::endl;
		exit(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Camera* Graphic::GetCamera()
{
	return mCamera;
}

GLuint Graphic::Get_FirstPassProgram()const
{
	return FirstPass;
}

GLint Graphic::Get_ShadowMapLocation()const
{
	return ShadowMapModel;
}

GLuint Graphic::Get_ForwardRednerProgram()const
{
	return ForwardRenderProg.GetProgramID();
}

void Graphic::TurnLight(bool on_off)
{
	isLightON_OFF = on_off;
}

void Graphic::SetMotionBlurr(bool isON)
{
	isMotionBlurr = isON;
}

void Graphic::DepthStart()
{
	mCamera->Update();
	//binf the depth buf
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	//glClearBufferfv(GL_COLOR, 0, float_zeros); // Clear buffer
	glClearBufferfv(GL_DEPTH, 0, float_ones);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//use the shadow map prog
	glUseProgram(SM_Light);

	mLightViewMtx = glm::lookAt(Vec3(0, 50, 10), Vec3(0, 0, 0), Vec3(0, 1, 0));
	
	glUniformMatrix4fv(ShadowMapProj, 1, GL_TRUE, glm::value_ptr(mPerspMtx));
	glUniformMatrix4fv(ShadowMapView, 1, GL_FALSE, glm::value_ptr(mLightViewMtx));
}
void Graphic::DepthEnd()
{
	//bind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

LightBlin* Graphic::GetLight()
{
	return mLighting;
}

GLuint Graphic::GetShadowDepth()const
{
	return depth_tex;
}

const Mat4& Graphic::GetLightViewMat()
{
	return mLightViewMtx;
}


