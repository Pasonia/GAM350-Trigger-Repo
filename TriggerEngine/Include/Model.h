/******************************************************************************/
/*!
\file Model.h
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
#ifndef MODEL_H_
#define MODEL_H_

#include "Animation.h"
#include "AnimationTimeTrack.h"

static const unsigned NUM_BONES_PER_VERTEX = 4;

struct VertexType
{
	VertexType();
	void AddBoneData(int BoneID, float Weight, VertexType &Bonedata);


	Vec4 Pos;
	Vec4 Nrm;
	Vec2 UV;

	int   boneIds[NUM_BONES_PER_VERTEX];
	float boneWeights[NUM_BONES_PER_VERTEX];
};

struct VertexLayoutElement
{
	int index;
	int size;
	int type;
	bool normalized;
	int offset;
};

const VertexLayoutElement vertexLayout[] =
{
	{ 0, 4, GL_FLOAT, GL_FALSE, offsetof(VertexType, Pos) },
	{ 1, 4, GL_FLOAT, GL_FALSE, offsetof(VertexType, Nrm) },
	{ 2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexType, UV) },
	{ 3, 4, GL_INT, GL_FALSE, offsetof(VertexType, boneIds) },
	{ 4, 4, GL_FLOAT, GL_FALSE, offsetof(VertexType, boneWeights) },
	{ 0 }
};

struct Texture
{
	Texture() :mTextureID(0), mID(-1){}
	GLuint mTextureID;
	int mID;
};


enum OBJECTFLAGS
{
	RENDER_WIREFRAME = 1,
	RENDER_AABB = 2,
	RENDER_OBB = 4,
	RENDER_HIGHLIGHT = 8,
	RENDER_BS = 16,
	RENDER_1 = 32,
	RENDER_2 = 64,
	RENDER_3 = 128,
	RENDER_4 = 256
};

class Model
{
public:
	Model();
	~Model();
	void Destroy();
	//rendering
	void Render(Mat4 &ModelMat, Texture* mTexture, int mFlag, int anim, float timer, const Vec2& scaling);
	void RenderShadowMap(Mat4 &ModelMat);
	void RenderGAOMAO(Mat4 &mFinalMtx, bool isCollided);
	void RednerSkybox(Mat4 &ModelMat, float id,float STATE);
	void ForwardRender(const Mat4 &ModelMat, const Texture* mTexture, const Vec2& scaling, const int Number, const Texture *tex,bool isTerrain = true);
	//Attribute binding
	void BindBuffer();
	void UnBindVAO();
	void ReBindVAO();
	//Model data
	std::vector<VertexType> mVertexData;
	std::vector<unsigned int> mIndexData;
	unsigned vertexCount;
	unsigned indexCount;
	//Animation Time Track data
	void AddAnimationTrackTimer(float start, float end);
	
	//Animation Data
	Animation mAnimationData;

	
private:
	//bind the buffer
	void BindAttributes();
	//FirstPass data
	void FirstPassProg();
	//shadowmap location
	void GetShadowMapLocation();
	//forward render location
	void ForwardRenderProg();
	//Render buffer
	GLuint mVertexArrayID;
	GLuint mVertexBuffer;
	GLuint mIndexBuffer;
	GLuint mDeferred_ModelLocation;
	GLuint mBoneMatrix_Location;
	GLuint mAnimation_Location;
	GLuint mHightLight_Location;
	GLuint mDebug_Location;
	GLuint mDebugColor_Location;
	static GLuint mFirstPassProg;
	AnimationTimeTrack m_animationtrack;
	//Shadow map
	Mat4 depthMVPMatrix;
	static GLuint mShadowMapLoc;
	static GLuint mForwardRenderProg;
};


#endif
