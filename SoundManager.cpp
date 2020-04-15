#include "SoundManager.h"

SoundManager::SoundManager(const char* bgmName)
	: mSystem(nullptr)
	, mSound(nullptr)
	, mChannel(nullptr)
	, mBIsPlaying(false)
{
	FMOD_System_Create(&mSystem);
	FMOD_System_Init(mSystem, 32, FMOD_INIT_NORMAL, nullptr);
	FMOD_System_CreateSound(mSystem, bgmName, FMOD_DEFAULT, 0, &mSound);
	FMOD_System_PlaySound(mSystem, mSound, nullptr, false, &mChannel);
}

SoundManager::~SoundManager()
{
	FMOD_Sound_Release(mSound);
	FMOD_System_Close(mSystem);
	FMOD_System_Release(mSystem);
}

void SoundManager::UpdateSound()
{
	FMOD_Channel_IsPlaying(mChannel, &mBIsPlaying);
	if (mBIsPlaying == 1)
	{
		FMOD_System_Update(mSystem);
	}
}