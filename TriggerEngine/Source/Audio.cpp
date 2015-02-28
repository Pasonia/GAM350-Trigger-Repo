/******************************************************************************/
/*!
\file Audio.cpp
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
#include "Audio.h"
#include "TriggerAssert.h"

SoundSystem * SoundSystem::m_instance = 0;

void CheckFMODError(FMOD_RESULT result)
{
	ASSERT(result == FMOD_OK, "FMOD error: %s\n", FMOD_ErrorString(result));
}

SoundSystem* SoundSystem::GetInstance()
{
	m_instance = m_instance ? m_instance : new SoundSystem();
	return m_instance;
}

SoundSystem::SoundSystem() : m_count(0)
{
	//Creates FMOD systems
	CheckFMODError(FMOD::System_Create(&m_system));

	//Version Check
	unsigned int ver;
	CheckFMODError(m_system->getVersion(&ver));

	ASSERT(ver == FMOD_VERSION, "FMOD version mismatch - DLL %d and Program %d", ver, FMOD_VERSION);

	//Initialize FMOD systems
	CheckFMODError(m_system->init(100, FMOD_INIT_NORMAL, 0));

	InitChannels();
}

SoundSystem::~SoundSystem()
{
	m_system->close();
	m_system->release();
}

void SoundSystem::LoadSound(const std::string& itemname, const std::string& filename, int flags, bool isBGM)
{
	AudioData data;
	data.index = m_sound.size();
	FMOD::Sound * sound;// = m_sound[data.index];
	FMOD::Channel * channelptr = m_channel[data.index];
	data.type = isBGM ? BGM : SFX;
	//m_sound.push_back(soundptr);
	//m_channel.push_back(channelptr);
	//FMOD_RESULT result;

	if (isBGM)
	{
		CheckFMODError(m_system->createStream(filename.c_str(), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound));
		m_sound.push_back(sound);
	}
	else
	{
		CheckFMODError(m_system->createStream(filename.c_str(), FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &sound));
		m_sound.push_back(sound);
		//CheckFMODError(channelptr->setChannelGroup(channelGroup[SFX]));
	}
	m_soundmap[itemname] = data;
}

void SoundSystem::InitChannels()
{
	CheckFMODError(m_system->createChannelGroup("Background Music", &channelGroup[BGM]));
	CheckFMODError(m_system->createChannelGroup("Sound Effects", &channelGroup[SFX]));
	CheckFMODError(m_system->getMasterChannelGroup(&channelGroup[MASTER]));

	CheckFMODError(channelGroup[MASTER]->addGroup(channelGroup[BGM]));
	CheckFMODError(channelGroup[MASTER]->addGroup(channelGroup[SFX]));
}

void SoundSystem::SetVolume(const float volume, const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		m_channel[m_soundmap[audio].index]->setVolume(volume);
	}
}
void SoundSystem::SetVolumeChannelGroup(const float volume, unsigned channelGroupNumber)
{
	channelGroup[channelGroupNumber]->setVolume(volume);
	std::cout << channelGroupNumber << " , " << volume << "\n";
}
float SoundSystem::GetVolume(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		float vol;
		m_channel[m_soundmap[audio].index]->getVolume(&vol);
		return vol;
	}
	return 0.f;
}
float SoundSystem::GetVolumeChannelGroup(unsigned channelGroupNumber)
{
	float vol;
	channelGroup[channelGroupNumber]->getVolume(&vol);
	return vol;
}

void SoundSystem::Pause(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		m_channel[m_soundmap[audio].index]->setPaused(true);
	}
}
void SoundSystem::Unpause(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		m_channel[m_soundmap[audio].index]->setPaused(false);
	}
}
void SoundSystem::StartPlayingSound(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		FMOD::Channel* channel;
		CheckFMODError(m_system->playSound(FMOD_CHANNEL_FREE, m_sound[m_soundmap[audio].index], false, &channel));
		m_channel[m_soundmap[audio].index] = channel; 
		CheckFMODError(m_channel[m_soundmap[audio].index]->setChannelGroup(channelGroup[m_soundmap[audio].type]));
	}
}
void SoundSystem::StopPlayingSound(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		m_channel[m_soundmap[audio].index]->stop();
	}
}
void SoundSystem::StopAllSounds()
{
	for (size_t i = 0; i < 1024; ++i)
	{
		m_channel[i]->stop();
	}
}
void SoundSystem::StopPlayingChannelGroup(unsigned channelGroupNumber)
{
	channelGroup[channelGroupNumber]->stop();
}
void SoundSystem::StopAllChannelGroups()
{
	for (unsigned i = 0; i < TOTAL_CHANNEL_GROUPS; ++i)
	{
		StopPlayingChannelGroup(i);
	}
}
bool SoundSystem::CheckPlaying(const std::string& audio)
{
	if (m_soundmap.find(audio) != m_soundmap.end())
	{
		bool result;
		m_channel[m_soundmap[audio].index]->isPlaying(&result);
		return result;
	}
	return false;
}

void SoundSystem::Update(float deltaTime)
{
	m_system->update();
}

void SoundSystem::ErrorCheck(FMOD_RESULT Result)
{
	ASSERT(Result != FMOD_OK, "Error in Sound System!");
}

void SoundSystem::Destroy()
{
	if (m_instance)
		delete m_instance;
}