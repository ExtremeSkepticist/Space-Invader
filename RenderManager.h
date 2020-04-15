#pragma once

#include <windows.h>

class RenderManager final
{
	friend class GameManager;

private:
	RenderManager();
	RenderManager(const RenderManager&) = delete;
	RenderManager(RenderManager&&) = delete;
	virtual ~RenderManager();

	RenderManager& operator=(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&&) = delete;

	void clearScreen();
	void flipScreen();
	void printToScreen(int x, int y, const char* str);
	void setColor(unsigned short color);

	unsigned int mScreenIndex;
	HANDLE mScreens[2];
};