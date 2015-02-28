/******************************************************************************/
/*!
\file ModelManager.cpp
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

#include "ModelManager.h"
#include "TriggerAssert.h"

ModelManager* ModelManager::mInstance = 0;

ModelManager* ModelManager::Inst()
{
	mInstance = mInstance ? mInstance : new ModelManager();
	return mInstance;
}

void ModelManager::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}


ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
	for (auto it = mModelContainer.begin(); it != mModelContainer.end(); ++it)
	{
		it->second.mAnimationData.Destroy();
		it->second.Destroy();
	}
	mModelContainer.clear();

	m_AABB.mAnimationData.Destroy();
	m_BS.mAnimationData.Destroy();
}

void ModelManager::CreateBoundingBox()
{
	ASSERT(ModelLoader_Inst->LoadAnimatedFile("../Resource/Model/Box.dae", m_AABB), "Cannot Load AABB");
	ASSERT(ModelLoader_Inst->LoadAnimatedFile("../Resource/Model/Sphere.dae", m_BS), "Cannot Load BS");
}

void ModelManager::CreateModel(const std::string & Path, const std::string &ObjName)
{
	auto itr = mModelContainer.find(ObjName);
	if (itr != mModelContainer.end())
		return;

	Model mModel;
	
	ASSERT(ModelLoader_Inst->LoadAnimatedFile(Path, mModel), "Cannot Load Animated Model");
	
	mModelContainer.insert(std::make_pair(ObjName, mModel));
}

void ModelManager::CreateModel(const std::string &ObjName, const std::vector<Vec4> &vcont)
{
	auto itr = mModelContainer.find(ObjName);
	if (itr != mModelContainer.end())
		return;

	Model mModel;

	ASSERT(ModelLoader_Inst->LoadModel(mModel, vcont), "Cannot Load static Model");

	mModelContainer.insert(std::make_pair(ObjName, mModel));
}

void ModelManager::CreateModel(const std::string &ObjName, const HeightMapData *HM_data, const Vec2 &diamension)
{
	auto itr = mModelContainer.find(ObjName);
	if (itr != mModelContainer.end())
		return;

	Model mModel;
	ASSERT(ModelLoader_Inst->LoadModel(mModel, HM_data, diamension), "Cannot Load static Model");
	mModelContainer.insert(std::make_pair(ObjName, mModel));
}

Model* ModelManager::GetModelData(const std::string& ObjName)
{
	auto itr = mModelContainer.find(ObjName);
	if (itr != mModelContainer.end())
	{
		return &(itr->second);
	}
	else
	{
		return NULL;
	}
}

void ModelManager::RenderDebugAABB(Mat4 transform, bool collided)
{
	m_AABB.RenderGAOMAO(transform, collided);
}
void ModelManager::RenderDebugBS(Mat4 transform, bool collided)
{
	m_BS.RenderGAOMAO(transform, collided);
}

