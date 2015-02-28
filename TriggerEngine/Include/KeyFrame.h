/******************************************************************************/
/*!
\file KeyFrame.h
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
#ifndef KEYFRAME_H_
#define KEYFRAME_H_

#include "TEMath.h"
#include "assimp\scene.h"

struct PositionKeyFrame
{
	PositionKeyFrame(float time, Vec4 position);
	PositionKeyFrame(aiVectorKey& vectorkey);

	float m_Time;
	Vec4 m_Position;
};

struct RotationKeyFrame
{
	RotationKeyFrame(float time, glm::quat rotation);
	RotationKeyFrame(aiQuatKey& vectorkey);

	float m_Time;
	glm::quat m_Rotation;
};

struct ScaleKeyFrame
{
	ScaleKeyFrame(float time, Vec4 scale);
	ScaleKeyFrame(aiVectorKey& vectorkey);

	float m_Time;
	Vec4 m_Scale;
};






#endif

