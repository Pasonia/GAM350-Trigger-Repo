/******************************************************************************/
/*!
\file BillBoardParticle.h
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

#ifndef BILLBOARDPARTICLE_H_
#define	BILLBOARDPARTICLE_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ShaderCompiler.h"

class BillBoardParticle
{
public:
	BillBoardParticle();
	~BillBoardParticle();
	bool Init();
	void StartShaderProgram();
	void SetUniform(const std::string& name, const Mat4 &mtx, int count = 1);
	void SetUniform(const std::string& name, const Vec3 &v3, int count = 1);

private:
	TriggerShader VertexRender, GeoRender,FragRender;
	TriggerShaderProg RenderParticleProg;
	bool isInitialise;
};





#endif
