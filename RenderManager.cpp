#include "RenderManager.h"

RenderManager::RenderManager()
	: mScreenIndex(0u)
{
	mScreens[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0
		, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	mScreens[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0
		, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	CONSOLE_CURSOR_INFO info = { 1, false };
	SetConsoleCursorInfo(mScreens[0], &info);
	SetConsoleCursorInfo(mScreens[1], &info);
}

RenderManager::~RenderManager()
{
	CloseHandle(mScreens[0]);
	CloseHandle(mScreens[1]);
}

void RenderManager::clearScreen()
{
	COORD coord = { 0, 0 };
	DWORD dword;

	FillConsoleOutputCharacter(mScreens[mScreenIndex], ' ', 80 * 25, coord, &dword);
}

void RenderManager::flipScreen()
{
	SetConsoleActiveScreenBuffer(mScreens[mScreenIndex]);
	mScreenIndex = !mScreenIndex;
}

void RenderManager::printToScreen(int x, int y, const char* str)
{
	DWORD dword;
	COORD cursorPos = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(mScreens[mScreenIndex], cursorPos);
	WriteFile(mScreens[mScreenIndex], str, strlen(str), &dword, NULL);
}

void RenderManager::setColor(unsigned short color)
{
	SetConsoleTextAttribute(mScreens[mScreenIndex], color);
}