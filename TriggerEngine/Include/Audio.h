/******************************************************************************/
/*!
\file Audio.h
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

#ifndef TRIGGERAUDIO_H_
#define TRIGGERAUDIO_H_

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum CHANNEL_TYPES
{
	MASTER = 0,
	BGM,
	SFX,
	TOTAL_CHANNEL_GROUPS
};

enum SOUND_FADE
{
	NO_FADE = 0,
	FADE_IN,
	FADE_OUT
};

struct AudioFade
{
	unsigned m_flag;
	float m_targetvolume;
	float m_rateofchange;
};

struct AudioData
{
	unsigned type;
	unsigned index;
};

typedef std::map<std::string, AudioData> SoundMap;

class SoundSystem
{
private:
	SoundSystem();
	~SoundSystem();

	FMOD::System *m_system;
	std::vector<FMOD::Sound*> m_sound;
	FMOD::Channel* m_channel[32];
	FMOD::ChannelGroup *channelGroup[TOTAL_CHANNEL_GROUPS];
	FMOD_RESULT result;
	SoundMap m_soundmap;
	unsigned m_count;

	//std::vector<AudioFade> m_audiofade;
	//AudioFade fadeChannelGroup[TOTAL_CHANNEL_GROUPS];

	//void CheckFade(float deltaTime);
	//void CheckChannelGroupFade(float deltaTime);
	
	static SoundSystem * m_instance;

public:
	static SoundSystem * GetInstance();
	static void Destroy();
	void LoadSound(const std::string& itemname, const std::string& filename, int flags, bool isBGM);
	void UnloadAllSounds();

	void InitChannels();

	void SetVolume(const float volume, const std::string& audio);
	void SetVolumeChannelGroup(const float volume, unsigned channelGroupNumber);
	float GetVolume(const std::string& audio);
	float GetVolumeChannelGroup(unsigned channelGroupNumber);

	void Pause(const std::string& audio);
	void Unpause(const std::string& audio);
	void StartPlayingSound(const std::string& audio);
	void StopPlayingSound(const std::string& audio);
	void StopAllSounds();
	void StopPlayingChannelGroup(unsigned channelGroupNumber);
	void StopAllChannelGroups();
	bool CheckPlaying(const std::string& audio);
	//void Fade(unsigned flag, unsigned channelNumber, float fadeAmount, float rateOfChange);
	//void FadeChannelGroup(unsigned flag, unsigned channelGroupNumber, float fadeAmount, float rateOfChange);
	//void FadeTargetVolumeChannelGroup(unsigned channelGroupNumber, float targetVolume, float rateOfChange);

	void Update(float deltaTime);
	void ErrorCheck(FMOD_RESULT Result);
};

#define SOUNDMGR_INST SoundSystem::GetInstance()

#endif