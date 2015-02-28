/******************************************************************************/
/*!
\file ModelManager.h
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
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <map>
#include "ModelLoader.h"

typedef std::map<std::string, Model> ModelContainer;


class ModelManager
{
public:
	static ModelManager* Inst();
	static void Destroy();

	void CreateBoundingBox();
	void CreateModel(const std::string & Path, const std::string &ObjName);
	void CreateModel(const std::string &ObjName, const std::vector<Vec4> &vcont);
	void CreateModel(const std::string &ObjName, const HeightMapData *HM_data, const Vec2 &diamension);
	Model* GetModelData(const std::string& ObjName);

	void RenderDebugAABB(Mat4 transform, bool collided);
	void RenderDebugBS(Mat4 transform, bool collided);
private:
	ModelContainer mModelContainer;
	static ModelManager* mInstance;
	//fixed debug models
	Model m_AABB, m_BS;
	ModelManager();
	~ModelManager();
};

#define ModelManager_Inst ModelManager::Inst()





#endif


