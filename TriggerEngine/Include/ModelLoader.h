/******************************************************************************/
/*!
\file ModelLoader.h
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
#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include "Model.h"
#include <map>
#include <string>
#include "TerrainHeight.h"

class ModelLoader
{
public:
	static ModelLoader* Inst();
	static void Destroy();
	//load Model from collada dae file
	bool LoadModel(const std::string FileName, Model &mModel);
	//draw basic 
	bool LoadModel(Model &mModel, const std::vector<Vec4> &vcont);
	//draw terrain
	bool LoadModel(Model &mModel, const HeightMapData *HM_data, const Vec2 &diamension);
	//load animation
	bool LoadAnimatedFile(const std::string& fileName, Model& mModel);

private:
	static ModelLoader* mInstance;
	ModelLoader();
	~ModelLoader();
	//load data
	bool InitFromScene(const aiScene* pScene, Model& pModel);
	void LoadBones(const aiMesh* pMesh, Model& pModel);
};

#define ModelLoader_Inst ModelLoader::Inst()







#endif


