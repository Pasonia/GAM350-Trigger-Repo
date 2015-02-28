/******************************************************************************/
/*!
\file Graphic.h
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
#ifndef _GRAPHICCLASS_H_
#define _GRAPHICCLASS_H_

#include "Opengl.h"
#include "Camera.h"
#include "LightShader.h"
#include "ShaderCompiler.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ShaderCompiler.h"
#include "Skybox.h"

#define MAX_NUM_BUFFER 4
#define DEPTH_TEXTURE_SIZE 2048
class Graphic
{
public:
	Graphic();
	~Graphic();

	bool DeferredStartUp(OpenGL *OpenGL_, unsigned int height, unsigned int width);
	void DeferredRenderStart();
	void DeferredRenderEnd();
	void DepthStart();
	void DepthEnd();
	void Rendering();
	//get first pass programe ID
	GLuint Get_FirstPassProgram()const;
	GLint Get_ShadowMapLocation()const;
	GLuint Get_ForwardRednerProgram()const;
	//set light on or off
	void TurnLight(bool on_off);
	void SetMotionBlurr(bool isON);
	Camera* GetCamera();
	LightBlin* GetLight();
	//get shadow depth
	GLuint GetShadowDepth()const;
	const Mat4& GetLightViewMat();
private:
	void CreateShadowBuffer(unsigned int width, unsigned int height);
	void CreateCamera();
	void CreateBlinPhong();
	//matrix for  view and perp
	Mat4 mPerspMtx, mLightViewMtx;

	OpenGL *mOpenGL;
	unsigned int mHeight, mWidth;
	Camera *mCamera;
	LightBlin *mLighting;
	GLuint gbuffer;
	GLuint gbuffer_tex[MAX_NUM_BUFFER];
	GLuint fs_quad_vao;
	//deferred first and second pass
	GLuint FirstPass;
	GLuint SecondPass;
	//shader program
	TriggerShaderProg FirstPassProg,SecondPassProg, ShadowLightProg, ForwardRenderProg;

	GLint deferred_viewLocation;
	GLint deferred_projLocation;
	//light ON_OFF
	bool isLightON_OFF;
	bool isMotionBlurr;
	//shadow map data
	GLuint SM_Light;
	GLuint depth_fbo;
	GLuint depth_tex;
	GLint ShadowMapModel;
	GLint ShadowMapProj;
	GLint ShadowMapView;
};







#endif



