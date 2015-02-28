#ifndef GRASS_SYSTEM_H_
#define	GRASS_SYSTEM_H_

#include "Model.h"
#include "GrassUpdate.h"
#include "GrassRender.h"

struct GrassType
{
	Vec3 mPosition;
	float mSize;
	int mType;
};

class GrassClass
{
public:
	GrassClass();
	~GrassClass();
	void InitGrass(const std::string& path, const Vec3 &pos, const float scale, int TEST_SIZE = 1000);
	void RenderGrass(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2);
	void UpdateGrass();

private:
	//shader progID
	//particle class update & render
	GrassUpdate mGrassUpdate;
	GrassRender mGrassRender;
	GLuint mTransformFeedbackBuffer;
	GLuint mQuery;
	//GLuint mQuery;
	GLuint mGrassBuffer[2];
	GLuint mVAO[2];
	//texture
	Texture mGrassTexture;
	int CurrentReadBuffer;	//current buffer, [0] or [1]
	int NumberOfGrass;
	//grass data
	Vec3 mGrassPosition;
	float mGrassSize;
};




#endif
