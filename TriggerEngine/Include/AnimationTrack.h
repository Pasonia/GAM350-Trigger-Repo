/******************************************************************************/
/*!
\file AnimationTrack.h
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

#ifndef ANIMATIONTRACK_H_
#define ANIMATIONTRACK_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "KeyFrame.h"

typedef std::vector<PositionKeyFrame> PositionKeyFrameListType;
typedef std::vector<RotationKeyFrame> RotationKeyFrameListType;
typedef std::vector<ScaleKeyFrame>	  ScaleKeyFrameListType;

struct PerBoneTrack
{
	//interpolation per frame
	void InterpolateKeyFrame(float animationTime, Mat4& animationMatrix);
	//data
	PositionKeyFrameListType m_PositionKeyFrameList;
	RotationKeyFrameListType m_RotationKeyFrameList;
	ScaleKeyFrameListType	 m_ScaleKeyFrameList;
};

typedef std::unordered_map<unsigned, PerBoneTrack> PerBoneTrackListType;

struct AnimationTrack
{
	//the matrix
	void GetInterpolatedMatrix(float animationTime, unsigned boneId, Mat4& transform);
	//data
	PerBoneTrackListType m_PerBoneTrackList;
	float m_Duration;
};

template<typename T>
unsigned FindIndexFromTime(float animationTime, T list)
{
	unsigned size = list.size() - 1;
	for (unsigned i = 0; i < size; i++) {
		if (animationTime < (float)list[i].m_Time)
		{
			return i;
		}
	}
	return 0;
}




#endif
