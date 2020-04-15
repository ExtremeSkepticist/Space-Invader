#pragma once

#include <fmod.h>

class SoundManager final
{
	friend class GameManager;

private:
	SoundManager() = delete;
	SoundManager(const char* bgmName);
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	virtual ~SoundManager();

	SoundManager& operator=(SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	void UpdateSound();

	FMOD_SYSTEM* mSystem;
	FMOD_SOUND* mSound;
	FMOD_CHANNEL* mChannel;
	FMOD_BOOL mBIsPlaying;
};