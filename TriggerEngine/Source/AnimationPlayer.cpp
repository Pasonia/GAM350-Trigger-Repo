/******************************************************************************/
/*!
\file AnimationPlayer.cpp
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
#include "AnimationPlayer.h"

AnimationTrack* AnimationPlayer::GetAnimationTrack(const std::string& animationTrackName)
{
	auto itr = m_AnimationTrackList.find(animationTrackName);
	if (itr != m_AnimationTrackList.end())
	{
		return &itr->second;
	}
	return NULL;
}

void AnimationPlayer::SetCurrentAnimationTrack(const std::string& animationTrackName)
{

}

void AnimationPlayer::PlayCurrentAnimation(float animationTime)
{

}

