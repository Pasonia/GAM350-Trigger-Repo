/******************************************************************************/
/*!
\file Animation.h
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

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ImageLoader.h"
#include <vector>
#include <map>

//Assimp
#include "assimp\Importer.hpp" 
#include "assimp\scene.h"
#include "assimp\postprocess.h"

struct BoneInfo
{
	BoneInfo();
	Mat4 BoneOffset;
	Mat4 FinalTransformation;
};

struct Animation
{
	//destroy
	void Destroy();
	//Load assimp
	void AssimpStartup(const std::string &filename);
	void BoneTransform(float TimeInSeconds, std::vector<Mat4>& Transforms);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Mat4& ParentTransform);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
	//scaling
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	//rotation
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	//translation
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	//data
	Mat4 m_GlobalInverseTransform;
	unsigned int m_NumOfBones;
	std::vector<BoneInfo> m_BoneInfo;
	std::map<std::string, int> m_BoneMapping;

	const aiScene* m_pScene;
	Assimp::Importer *mImporter;

};





#endif





