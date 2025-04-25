#include "SoundManager.h"
#include "TimerManager.h"

HRESULT SoundManager::Init()
{
	System = nullptr;

	BgmVolume = 0.1f;
	EffectVolume = 0.1f;
	UIVolume = 0.1f;
	EnvVolume = 0.1f;
	BGMCurID = 0;
	BGMNextID = 0;
	EnvCurID = 0;
	EnvNextID = 0;

	BGMDelay = 1.f;
	EnvDelay = 1.f;

	BGMNextSound = nullptr;
	EnvNextSound = nullptr;

	pitch = 1.f;
	timer = 0.f;
	for (auto& channel : ChannelArr)
		channel = nullptr;

	if (FMOD_OK != FMOD_System_Create(&System, FMOD_VERSION))
		return E_FAIL;

	if (FMOD_OK != FMOD_System_Init(System, 255, FMOD_INIT_NORMAL, NULL))
		return E_FAIL;

	return S_OK;
}

void SoundManager::Update()
{
	if (BGMCurID != BGMNextID)
	{
		BGMDelay -= TimerManager::GetInstance()->GetDeltaTime();
		if (BGMDelay < 0.f)
		{
			BGMDelay = 0.f;
			
			BGMCurID = BGMNextID;
			StopSound(EChannelType::BGM);

			FMOD_System_PlaySound(System, BGMNextSound, nullptr, FALSE, &ChannelArr[(int)EChannelType::BGM]);
			FMOD_Channel_SetMode(ChannelArr[(int)EChannelType::BGM], FMOD_LOOP_NORMAL);

			BGMNextSound = nullptr;
		}
	}
	else
	{
		BGMDelay += TimerManager::GetInstance()->GetDeltaTime();
		if (BGMDelay > 1.f)
			BGMDelay = 1.f;
	}

	if (EnvCurID != EnvNextID)
	{
		EnvDelay -= TimerManager::GetInstance()->GetDeltaTime();
		if (EnvDelay < 0.f)
		{
			EnvDelay = 0.f;

			EnvCurID = EnvNextID;
			StopSound(EChannelType::Environment);

			FMOD_System_PlaySound(System, EnvNextSound, nullptr, FALSE, &ChannelArr[(int)EChannelType::Environment]);
			FMOD_Channel_SetMode(ChannelArr[(int)EChannelType::Environment], FMOD_LOOP_NORMAL);

			EnvNextSound = nullptr;
		}
	}
	else
	{
		EnvDelay += TimerManager::GetInstance()->GetDeltaTime();
		if (EnvDelay > 1.f)
			EnvDelay = 1.f;
	}
	FMOD_Channel_SetPitch(ChannelArr[(int)EChannelType::BGM], pitch);
	FMOD_Channel_SetVolume(ChannelArr[(int)EChannelType::BGM], BGMDelay * BgmVolume);
	FMOD_Channel_SetVolume(ChannelArr[(int)EChannelType::Environment], EnvDelay * EnvVolume);
	FMOD_Channel_SetVolume(ChannelArr[(int)EChannelType::Effect], EffectVolume);
	FMOD_Channel_SetVolume(ChannelArr[(int)EChannelType::UI], UIVolume);

	FMOD_System_Update(System);
}

void SoundManager::Release()
{
	for (auto& sound : MapSound)
		FMOD_Sound_Release(sound.second);

	MapSound.clear();
	if (System != nullptr)
	{
		FMOD_System_Release(System);
		FMOD_System_Close(System);
	}	

	ReleaseInstance();
}

HRESULT SoundManager::AddSound(string InSoundKey, const char* InSoundPath)
{
	FMOD_SOUND* Sound = nullptr;
	FMOD_RESULT Result = FMOD_System_CreateSound(System, InSoundPath, FMOD_LOOP_OFF, 0, &Sound);
	if (Result == FMOD_OK)
		MapSound.emplace(InSoundKey, Sound);
	else
		return E_FAIL;

	return S_OK;
}

void SoundManager::PlaySounds(string InSoundKey, EChannelType InChannelType)
{
	auto iter = MapSound.find(InSoundKey);
	if (iter == MapSound.end())
		return;

	FMOD_System_PlaySound(System, iter->second, nullptr, FALSE, &ChannelArr[(int)InChannelType]);
	FMOD_System_Update(System);
}

void SoundManager::PlayBGM(string InSoundKey)
{
	int index = 1;
	for (auto& iter : MapSound)
	{
		if (iter.first == InSoundKey)
		{
			BGMNextID = index;
			BGMNextSound = iter.second;
			break;
		}
		++index;
	}
}

void SoundManager::PlayEnv(string InSoundKey)
{
	int index = 1;
	for (auto& iter : MapSound)
	{
		if (iter.first == InSoundKey)
		{
			EnvNextID = index;
			EnvNextSound = iter.second;
			break;
		}
		++index;
	}
}

void SoundManager::PitchDown(EChannelType InChannelType)
{
	pitch = 0.6f;
}

void SoundManager::PitchOrigin(EChannelType InChannelType)
{
	pitch = 1.0f;
}

void SoundManager::StopSound(EChannelType InChannelType)
{
	FMOD_Channel_Stop(ChannelArr[(int)InChannelType]);
}

void SoundManager::StopAll()
{
	for(int i = 0; i < (int)EChannelType::MaxChannel; ++i)
		FMOD_Channel_Stop(ChannelArr[i]);
}


