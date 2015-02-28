/******************************************************************************/
/*!
\file AnimationTrack.cpp
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

#include "AnimationTrack.h"

void PerBoneTrack::InterpolateKeyFrame(float animationTime, Mat4& animationMatrix)
{
	// linearly interpolate the position
	unsigned positionIndex = FindIndexFromTime(animationTime, m_PositionKeyFrameList);
	PositionKeyFrame pre_Position = m_PositionKeyFrameList[positionIndex];
	PositionKeyFrame next_Position = m_PositionKeyFrameList[positionIndex + 1];

	// compute the factor to interpolate with 
	float deltaTime = next_Position.m_Time - pre_Position.m_Time;
	float InterpolationFactor = (animationTime - pre_Position.m_Time) / deltaTime;

	Vec4 interpolatedPosition = pre_Position.m_Position + (InterpolationFactor * (next_Position.m_Position - pre_Position.m_Position));

	// quaternion interpolation
	unsigned rotationIndex = FindIndexFromTime(animationTime, m_RotationKeyFrameList);
	RotationKeyFrame pre_Rotation = m_RotationKeyFrameList[rotationIndex];
	RotationKeyFrame next_Rotation = m_RotationKeyFrameList[rotationIndex + 1];

	// compute the factor to interpolate with 
	deltaTime = next_Rotation.m_Time - pre_Rotation.m_Time;
	InterpolationFactor = (animationTime - pre_Rotation.m_Time) / deltaTime;

	glm::quat interpolatedRotation;
	//&pre_Rotation.m_Rotation, &next_Rotation.m_Rotation, InterpolationFactor);
	interpolatedRotation = glm::mix(pre_Rotation.m_Rotation, next_Rotation.m_Rotation, InterpolationFactor);

	// linearly interpolate the scaling
	unsigned scaleIndex = FindIndexFromTime(animationTime, m_ScaleKeyFrameList);
	ScaleKeyFrame pre_Scale = m_ScaleKeyFrameList[scaleIndex];
	ScaleKeyFrame next_Scale = m_ScaleKeyFrameList[scaleIndex + 1];

	deltaTime = next_Scale.m_Time - pre_Scale.m_Time;
	InterpolationFactor = (animationTime - pre_Scale.m_Time) / deltaTime;

	Vec4 interpolatedScale = pre_Scale.m_Scale + (InterpolationFactor * (next_Scale.m_Scale - pre_Scale.m_Scale));

	// Build the matrix here
	Mat4 scale, rotation, translate;

	rotation = glm::mat4_cast(interpolatedRotation);
	glm::scale(scale, Vec3(interpolatedScale.x, interpolatedScale.y, interpolatedScale.z));
	glm::translate(translate, Vec3(interpolatedPosition.x, interpolatedPosition.y, interpolatedPosition.z));

	// the new interpolated matrix!!
	animationMatrix = scale * rotation * translate;
}

void AnimationTrack::GetInterpolatedMatrix(float animationTime, unsigned boneId, Mat4& transform)
{
	auto itr = m_PerBoneTrackList.find(boneId);
	if (itr != m_PerBoneTrackList.end()) // if this bone have keyframe
	{
		if (animationTime > m_Duration)
		{
			animationTime = std::fmod(animationTime, m_Duration);
		}
		itr->second.InterpolateKeyFrame(animationTime, transform); //replace the bind pose transform with the interpolated transform
	}
	return; // if no keyframe, just return. The input transform should be initialize to the bind pose transform
}
