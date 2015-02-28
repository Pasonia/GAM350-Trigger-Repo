/******************************************************************************/
/*!
\file ModelLoader.cpp
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

#include "ModelLoader.h"
#include "Primitive.h"

ModelLoader* ModelLoader::mInstance = 0;

ModelLoader* ModelLoader::Inst()
{
	mInstance = mInstance ? mInstance : new ModelLoader();

	return mInstance;
}

void ModelLoader::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

bool ModelLoader::LoadModel(const std::string FileName, Model &mModel)
{
	mModel.mAnimationData.AssimpStartup(FileName);

	//grab vertex
	std::vector<VertexType>& vertexList = mModel.mVertexData;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiMesh* paiMesh = mModel.mAnimationData.m_pScene->mMeshes[0];
	bool z_up = (FileName.find("_z") != std::string::npos);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		VertexType v;

		if (!z_up)
			v.Pos = Vec4(pPos->x, pPos->y, pPos->z, 1.0f);
		else
			v.Pos = Vec4(pPos->x, pPos->z, pPos->y, 1.0f);
		v.Nrm = Vec4(pNormal->x, pNormal->y, pNormal->z, 0.0f);
		v.UV  = Vec2(pTexCoord->x, pTexCoord->y);

		vertexList.push_back(v);
	}

	unsigned indexCount = paiMesh->mNumFaces * 3;
	std::vector<unsigned>& indexList = mModel.mIndexData;
	indexList.reserve(indexCount);

	unsigned int j = 0;
	for (unsigned int i = 0; j < indexCount; ++i)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		indexList.push_back(Face.mIndices[0]);
		indexList.push_back(Face.mIndices[1]);
		indexList.push_back(Face.mIndices[2]);
		j += 3;
	}

	mModel.vertexCount = vertexList.size();
	mModel.indexCount = indexList.size();
	//bind the vertex and index buffer
	mModel.BindBuffer();
	//unbind
	mModel.UnBindVAO();

	return true;
	//assimp will destroy their side stuff
}

bool ModelLoader::LoadModel(Model &mModel, const std::vector<Vec4> &vcont)
{
	std::vector<VertexType>& vertexList = mModel.mVertexData;
	unsigned int size_ = vcont.size();
	vertexList.reserve(size_);
	for (unsigned i = 0; i < size_; ++i)
	{
		VertexType v;
		v.Pos = vcont[i];
		v.Nrm = Vec4(1, 0, 0, 0.0f);
		v.UV = Vec2(0.5f, 0.5f);
		vertexList.push_back(v);
	}

	std::vector<unsigned>& indexList = mModel.mIndexData;
	indexList.reserve(size_);
	for (unsigned i = 0; i < size_; ++i)
		indexList.push_back(i);

	mModel.vertexCount = vertexList.size();
	mModel.indexCount = indexList.size();
	//bind the vertex and index buffer
	mModel.BindBuffer();
	//unbind
	mModel.UnBindVAO();
	mModel.mAnimationData.m_pScene = 0;
	return true;
}

bool ModelLoader::LoadModel(Model &mModel, const HeightMapData *HM_data, const Vec2 &diamension)
{
	std::vector<VertexType>& vertexList = mModel.mVertexData;
	std::vector<unsigned int>& indexList = mModel.mIndexData;

	int m_terrainWidth  = static_cast<int>(diamension.x);
	int m_terrainHeight = static_cast<int>(diamension.y);

	int m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
	int m_indexCount = m_vertexCount;

	vertexList.reserve(m_vertexCount);
	indexList.reserve(m_indexCount);

	int index = 0;

	for(int j=0; j<(m_terrainHeight-1); j++)
	{
		for(int i=0; i<(m_terrainWidth-1); i++)
		{
			int index1 = (m_terrainWidth * j) + i;             // Bottom left.
			int index2 = (m_terrainWidth * j) + (i + 1);       // Bottom right.
			int index3 = (m_terrainWidth * (j + 1)) + i;       // Upper left.
			int index4 = (m_terrainWidth * (j + 1)) + (i + 1); // Upper right.
			VertexType vt_t;
			unsigned int index_t;
			vt_t.Pos = Vec4(HM_data[index1].pos.x, HM_data[index1].pos.y, HM_data[index1].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index1].UV.x, HM_data[index1].UV.y);
			vt_t.Nrm = Vec4(HM_data[index1].nrm.x, HM_data[index1].nrm.y, HM_data[index1].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);

			// Bottom left.
			vt_t.Pos = Vec4(HM_data[index3].pos.x, HM_data[index3].pos.y, HM_data[index3].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index3].UV.x, HM_data[index3].UV.y);
			vt_t.Nrm = Vec4(HM_data[index3].nrm.x, HM_data[index3].nrm.y, HM_data[index3].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);

			vt_t.Pos = Vec4(HM_data[index2].pos.x, HM_data[index2].pos.y, HM_data[index2].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index2].UV.x, HM_data[index2].UV.y);
			vt_t.Nrm = Vec4(HM_data[index2].nrm.x, HM_data[index2].nrm.y, HM_data[index2].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);
			// Bottom left.
			vt_t.Pos = Vec4(HM_data[index3].pos.x, HM_data[index3].pos.y, HM_data[index3].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index3].UV.x, HM_data[index3].UV.y);
			vt_t.Nrm = Vec4(HM_data[index3].nrm.x, HM_data[index3].nrm.y, HM_data[index3].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);
			vt_t.Pos = Vec4(HM_data[index4].pos.x, HM_data[index4].pos.y, HM_data[index4].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index4].UV.x, HM_data[index4].UV.y);
			vt_t.Nrm = Vec4(HM_data[index4].nrm.x, HM_data[index4].nrm.y, HM_data[index4].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);
			vt_t.Pos = Vec4(HM_data[index2].pos.x, HM_data[index2].pos.y, HM_data[index2].pos.z, 1.0f);
			vt_t.UV = Vec2(HM_data[index2].UV.x, HM_data[index2].UV.y);
			vt_t.Nrm = Vec4(HM_data[index2].nrm.x, HM_data[index2].nrm.y, HM_data[index2].nrm.z, 1.0f);
			index_t = index;
			index++;
			vertexList.push_back(vt_t);
			indexList.push_back(index_t);
		}
	}
	mModel.vertexCount = m_vertexCount;
	mModel.indexCount = m_indexCount;
	//bind the vertex and index buffer
	mModel.BindBuffer();
	//unbind
	mModel.UnBindVAO();
	mModel.mAnimationData.m_pScene = 0;

	return true;
}

void ModelLoader::LoadBones(const aiMesh* pMesh, Model& pModel)
{
	for (unsigned int i = 0; i < pMesh->mNumBones; i++) 
	{
		int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (pModel.mAnimationData.m_BoneMapping.find(BoneName) == pModel.mAnimationData.m_BoneMapping.end())
		{
			// Allocate an index for a new bone
			BoneIndex = pModel.mAnimationData.m_NumOfBones;
			++pModel.mAnimationData.m_NumOfBones;
			BoneInfo bi;
			pModel.mAnimationData.m_BoneInfo.push_back(bi);
			copyAiMatrixToGLM(&pMesh->mBones[i]->mOffsetMatrix, pModel.mAnimationData.m_BoneInfo[BoneIndex].BoneOffset);
			pModel.mAnimationData.m_BoneMapping[BoneName] = BoneIndex;
		}
		else 
		{
			BoneIndex = pModel.mAnimationData.m_BoneMapping[BoneName];
		}
		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) 
		{
			int VertexID = pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			pModel.mVertexData[VertexID].AddBoneData(BoneIndex, Weight, pModel.mVertexData[VertexID]);
		}
	}
}

bool ModelLoader::InitFromScene(const aiScene* pScene, Model& pModel)
{
	//grab vertex
	std::vector<VertexType>& vertexList = pModel.mVertexData;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiMesh* paiMesh = pScene->mMeshes[0];

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		VertexType v;

		v.Pos = Vec4(pPos->x, pPos->y, pPos->z, 1.0f);
		v.Nrm = Vec4(pNormal->x, pNormal->y, pNormal->z, 0.0f);
		v.UV = Vec2(pTexCoord->x, pTexCoord->y);

		vertexList.push_back(v);
	}

	unsigned indexCount = paiMesh->mNumFaces * 3;
	std::vector<unsigned>& indexList = pModel.mIndexData;
	indexList.reserve(indexCount);

	unsigned int j = 0;
	for (unsigned int i = 0; j < indexCount; ++i)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		indexList.push_back(Face.mIndices[0]);
		indexList.push_back(Face.mIndices[1]);
		indexList.push_back(Face.mIndices[2]);
		j += 3;
	}
	//load bones
	LoadBones(paiMesh, pModel);

	pModel.vertexCount = vertexList.size();
	pModel.indexCount = indexList.size();
	//bind the vertex and index buffer
	pModel.BindBuffer();
	//unbind
	pModel.UnBindVAO();
	return true;
}

bool ModelLoader::LoadAnimatedFile(const std::string& fileName, Model& mModel)
{
	mModel.mAnimationData.AssimpStartup(fileName);

	bool Ret;

	Mat4 tempMat;
	copyAiMatrixToGLM(&mModel.mAnimationData.m_pScene->mRootNode->mTransformation, tempMat);
	mModel.mAnimationData.m_GlobalInverseTransform = glm::inverse(tempMat);
	//init the data
	mModel.mAnimationData.m_NumOfBones = 0;
	Ret = InitFromScene(mModel.mAnimationData.m_pScene, mModel);

	return Ret;
}

ModelLoader::ModelLoader()
{

}
ModelLoader::~ModelLoader()
{

}
