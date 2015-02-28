/******************************************************************************/
/*!
\file AnimationPlayer.h
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

#ifndef ANIMATIONPLAYER_H_
#define ANIMATIONPLAYER_H_

#include "AnimationTrack.h"

typedef std::unordered_map<std::string, AnimationTrack> AnimationTrackListType;

struct AnimationPlayer
{
	AnimationTrack* GetAnimationTrack(const std::string& animationTrackName);

	// convenience function
	void SetCurrentAnimationTrack(const std::string& animationTrackName);
	void PlayCurrentAnimation(float animationTime);

	AnimationTrackListType m_AnimationTrackList;
	std::string m_CurrentAnimationTrackName;
	float m_CurrentAnimationTime;
};





#endif


