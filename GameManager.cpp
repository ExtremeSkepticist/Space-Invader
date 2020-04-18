#include <cassert>
#include <ctime>
#include <windows.h>
#include <random>

#include "GameManager.h"

GameManager::GameManager()
	: mSoundManager("undertale-ost-ghost-fight-extended.mp3")
	, mbIsGameOver(false)
	, mPrevTime(clock())
{
	static_assert(mMAP_START_X >= 0u, "Starting X point is negative");
	static_assert(mMAP_START_Y >= 0u, "Starting Y point is negative");
	static_assert(mMAP_WIDTH > 0u && mMAP_HEIGHT > 0u
		, "The Width or height of the map is equal to or less than zero");

	// Initializes the map
	for (size_t i = 0u; i < mMAP_HEIGHT; ++i)
	{
		for (size_t j = 0u; j < mMAP_WIDTH; ++j)
		{
			mMap[i][j] = eObjectType::EMPTY;
		}
	}

	// Sets up boundaries
	for (size_t i = 0u; i < mMAP_WIDTH; ++i)
	{
		mMap[0][i] = eObjectType::WALL;
		mMap[mMAP_HEIGHT - 1][i] = eObjectType::WALL;
	}
	for (size_t i = 1u; i < mMAP_HEIGHT; ++i)
	{
		mMap[i][0] = eObjectType::WALL;
		mMap[i][mMAP_WIDTH - 1] = eObjectType::WALL;
	}

	// Generates enemies
	for (size_t i = 23u; i < 25u; ++i)
	{
		mMap[1][i] = eObjectType::ENEMY;
		mMap[2][i] = eObjectType::ENEMY;
	}

	// Generates a player character
	mMap[12][mMAP_WIDTH / 2] = eObjectType::PLAYER;
}

void GameManager::Update(bool bIsLeftKeyPressed, bool bIsRightKeyPressed, bool enemyMove, bool bIsShoot)
{
	// Generates new enemies
	clock_t curTime = clock();
	constexpr clock_t ENEMY_GENERATE_INTERVAL = 4500U;
	if (curTime - mPrevTime > ENEMY_GENERATE_INTERVAL)
	{
		size_t generatePoint = rand() % 23u + 10u;
		size_t bias = rand() % 5 + 1;

		mMap[1][generatePoint - bias] = eObjectType::ENEMY;
		mMap[1][generatePoint] = eObjectType::ENEMY;
		mMap[1][generatePoint + bias] = eObjectType::ENEMY;
		
		mPrevTime = clock();
	}

	// Bullet collsion handling
	for (size_t i = 1u; i < mMAP_HEIGHT - 1u; ++i)
	{
		for (size_t j = 1u; j < mMAP_WIDTH - 1u; ++j)
		{
			if (mMap[i][j] == eObjectType::BULLET)
			{
				mMap[i][j] = eObjectType::EMPTY;

				if (mMap[i - 1][j] == eObjectType::ENEMY)
				{
					mMap[i - 1][j] = eObjectType::EMPTY;
				}
				else if (mMap[i - 1][j] == eObjectType::EMPTY)
				{
					mMap[i - 1][j] = eObjectType::BULLET;
				}
			}
		}
	}

	// Updates movable objects location
	for (size_t i = mMAP_HEIGHT - 2u; i > 0u; --i)
	{
		for (size_t j = 0u; j < mMAP_WIDTH - 1u; ++j)
		{
			// Updates enemies location
			if (enemyMove && mMap[i][j] == eObjectType::ENEMY)
			{
				// Set Gameover flag if an enemy hits the ground 
				// or walks into a player character
				if ((mMap[i + 1][j] == eObjectType::PLAYER || mMap[i + 1][j] == eObjectType::WALL)
					&& !mbIsGameOver)
				{
					mbIsGameOver = true;
					return;
				}

				mMap[i][j] = eObjectType::EMPTY;
				mMap[i + 1][j] = eObjectType::ENEMY;
			}
			// Update player character location or shoots a bullet if requested
			else if (mMap[i][j] == eObjectType::PLAYER)
			{
				if (bIsShoot && mMap[i - 1][j] == eObjectType::EMPTY)
				{
					mMap[i - 1][j] = eObjectType::BULLET;
				}

				if (bIsLeftKeyPressed && j > 1u)
				{
					mMap[i][j] = eObjectType::EMPTY;
					mMap[i][j - 1] = eObjectType::PLAYER;
					bIsLeftKeyPressed = false;
				}
				else if (bIsRightKeyPressed && j < mMAP_WIDTH - 2u)
				{
					mMap[i][j] = eObjectType::EMPTY;
					mMap[i][j + 1] = eObjectType::PLAYER;
					bIsRightKeyPressed = false;
				}
			}
		}
	}

	mSoundManager.UpdateSound();
}

void GameManager::Render()
{
	if (mbIsGameOver)
	{
		MessageBox(0, "You Died", "Game Over", MB_OK);
		return;
	}

	mRenderManager.clearScreen();

	mRenderManager.printToScreen(mMAP_START_X, 20, "Move to left: press z");
	mRenderManager.printToScreen(mMAP_START_X, 21, "Move to right: press c");
	mRenderManager.printToScreen(mMAP_START_X, 22, "Shoot a bullet: press x");

	for (size_t i = 0u; i < mMAP_HEIGHT; ++i)
	{
		for (size_t j = 0u; j < mMAP_WIDTH; ++j)
		{
			if (mMap[i][j] == eObjectType::WALL)
			{
				mRenderManager.printToScreen(j + mMAP_START_X, i + mMAP_START_Y, "*");
			}
			else if (mMap[i][j] == eObjectType::ENEMY)
			{
				mRenderManager.printToScreen(j + mMAP_START_X, i + mMAP_START_Y, "@");
			}
			else if (mMap[i][j] == eObjectType::EMPTY)
			{
				mRenderManager.printToScreen(j + mMAP_START_X, i + mMAP_START_Y, " ");
			}
			else if (mMap[i][j] == eObjectType::PLAYER)
			{
				mRenderManager.printToScreen(j + mMAP_START_X, i + mMAP_START_Y, "#");
			}
			else if (mMap[i][j] == eObjectType::BULLET)
			{
				mRenderManager.printToScreen(j + mMAP_START_X, i + mMAP_START_Y, "o");
			}
		}
	}
	mRenderManager.flipScreen();
}

void GameManager::Wait() const
{
	clock_t prevTime;
	clock_t curTime;
	constexpr unsigned int FRAME_COUNT = 100U;

	prevTime = clock();
	while (true)
	{
		curTime = clock();
		if (curTime - prevTime > FRAME_COUNT)
		{
			return;
		}
	}
}