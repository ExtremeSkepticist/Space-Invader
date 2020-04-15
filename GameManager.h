#pragma once

#include "eObjectType.h"
#include "RenderManager.h"
#include "SoundManager.h"

class GameManager final
{
public:
	GameManager();
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	virtual ~GameManager() = default;

	GameManager& operator=(GameManager&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	void Update(bool bIsLeftKeyPressed, bool bIsRightKeyPressed, bool enemyMove, bool bIsShoot);
	void Render();
	void Wait() const;
	inline bool IsGameOver() { return mbIsGameOver; }

private:
	RenderManager mRenderManager;
	SoundManager mSoundManager;

	static constexpr unsigned int mMAP_START_X = 30U;
	static constexpr unsigned int mMAP_END_X = 75U;
	static constexpr unsigned int mMAP_START_Y = 5U;
	static constexpr unsigned int mMAP_END_Y = 18U;
	static constexpr unsigned int mMAP_WIDTH = mMAP_END_X - mMAP_START_X + 1U;
	static constexpr unsigned int mMAP_HEIGHT = mMAP_END_Y - mMAP_START_Y + 1U;
	
	eObjectType mMap[mMAP_HEIGHT][mMAP_WIDTH];

	bool mbIsGameOver;
	clock_t mOldTime;
};