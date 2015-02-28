/******************************************************************************/
/*!
\file AnimationTimeTrack.h
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

#ifndef ANIMATIONTIMETRACK_H_
#define ANIMATIONTIMETRACK_H_

#include "Timer.h"
#include <vector>

struct Track
{
	float start;
	float end;
};

class Model;

class AnimationTimeTrack
{
public:
	AnimationTimeTrack(Model * parent);
	~AnimationTimeTrack();
	void AddAnimationData(float start, float end);
	size_t GetAnimateDataCount();
	float GetTime(int anim_track, float& timer);
private:
	Model * m_parent;
	std::vector<Track> m_animationtracks;
};

#endif