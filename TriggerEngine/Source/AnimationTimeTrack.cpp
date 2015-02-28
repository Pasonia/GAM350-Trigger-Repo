/******************************************************************************/
/*!
\file AnimationTimeTrack.cpp
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
#include "AnimationTimeTrack.h"

AnimationTimeTrack::AnimationTimeTrack(Model * parent) :
m_parent(parent)

{
}
AnimationTimeTrack::~AnimationTimeTrack()
{

}

void AnimationTimeTrack::AddAnimationData(float start, float end)
{
	Track anim_data;
	anim_data.start = start;
	anim_data.end = end;
	m_animationtracks.push_back(anim_data);
}
size_t AnimationTimeTrack::GetAnimateDataCount()
{
	return m_animationtracks.size();
}
float AnimationTimeTrack::GetTime(int anim_track, float& timer)
{
	if (anim_track < 0 || anim_track >= static_cast<int>(m_animationtracks.size()))
		return 0.f;
	
	float duration = m_animationtracks[anim_track].end - m_animationtracks[anim_track].start;
	if (timer > duration) timer -= duration;

	return m_animationtracks[anim_track].start + timer;
}