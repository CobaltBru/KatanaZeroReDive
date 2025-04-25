#pragma once
#include "Reference\Headers\fmod\fmod.h"
#pragma comment(lib, "Reference/Librarys/fmod_vc")

#include "Singleton.h"
#include "config.h"
#include <unordered_map>

enum class EChannelType : uint8_t
{
	BGM,
	Effect,
	UI,
	Environment,
	MaxChannel = 32
};

class SoundManager : public Singleton<SoundManager>
{
public:
	HRESULT Init();
	void Update();
	void Release();

	HRESULT AddSound(string InSoundKey, const char* InSoundPath);

	void PlaySounds(string InSoundKey, EChannelType InChannelType);	
	void PlayBGM(string InSoundKey);	//배경
	void PlayEnv(string InSoundKey);	//환경음?

	void StopSound(EChannelType InChannelType);
	void StopAll();
private:
	unordered_map<string, FMOD_SOUND*> MapSound;
	FMOD_CHANNEL* ChannelArr[(int)EChannelType::MaxChannel];
	FMOD_SYSTEM* System;

	float BgmVolume;
	float EffectVolume;
	float UIVolume;
	float EnvVolume;

	FMOD_SOUND* BGMNextSound;
	FMOD_SOUND* EnvNextSound;

	int BGMCurID;
	int BGMNextID;
	int EnvCurID;
	int EnvNextID;

	float BGMDelay;
	float EnvDelay;
};

