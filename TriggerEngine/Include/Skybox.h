/******************************************************************************/
/*!
\file Skybox.h
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
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Opengl.h"
#include "ShaderCompiler.h"
#include "TEMath.h"
#include "ImageLoader.h"
#include "TriggerAssert.h"

class SkyBox
{
public:
	static SkyBox* Inst();
	static void Destroy();
	void CreateSkyBox();
	void Render(const Mat4 &modelMat, const Mat4 &viewMat, const Mat4 &projMat, float id, float State);

private:
	enum SKYBOX_INDEX
	{
		RIGHT = 0,
		LEFT,
		TOP,
		DOWN,
		FRONT,
		BACK,
		TOTAL_SIZE
	};

	SkyBox();
	~SkyBox();
	void LoadCubeMap();
	//Skybox prog ID
	TriggerShaderProg skyboxProg;
	//view in skybox
	GLuint ModelMatLoc,viewMatLoc,ProjMatLoc, time_step, mState;
	//cubeMap ID
	GLuint cubeTexID[3];
	static SkyBox* mInstance;
};

#define SkyBoxInst SkyBox::Inst()




#endif
