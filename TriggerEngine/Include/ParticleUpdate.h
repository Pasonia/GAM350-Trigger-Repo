/******************************************************************************/
/*!
\file ParticleUpdate.h
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
#ifndef PARTICLE_UPDATE_H_
#define	PARTICLE_UPDATE_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ShaderCompiler.h"
#include <string>

#define NUM_PARTICLE_ATTRIBUTES 6
#define FOR(q,n) for(int q=0;q<n;q++)

class ParticleUpdate
{
public:
	ParticleUpdate();
	~ParticleUpdate();
	bool Init();
	void StartShaderProgram();
	void SetUniform(const std::string& name, const Vec3 &v3, int count = 1);
	void SetUniform(const std::string& name, const float f, int count = 1);
	void SetUniform(const std::string& name, const int ival);
	//shader progID
	TriggerShaderProg UpdateParticleProg;

private:
	TriggerShader VertexUpdate, GeoUpdate;
	bool isInitialise;
};






#endif
