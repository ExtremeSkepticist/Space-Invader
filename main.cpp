#include <conio.h>
#include <ctime>

#include "GameManager.h"

int main()
{
	GameManager gameManager;
	clock_t prevTime = clock();
	constexpr clock_t TIME_INTERVAL = 1000U;

	while (!gameManager.IsGameOver())
	{
		bool bIsLeftKeyPressed = false;
		bool bIsRightKeyPressed = false;
		bool bIsShoot = false;
		bool enemyMove = false;

		if (_kbhit())
		{
			int keyPressed = _getch();
			switch (keyPressed)
			{
			case 'z':
			case 'Z':
				bIsLeftKeyPressed = true;
				break;
			case 'c':
			case 'C':
				bIsRightKeyPressed = true;
				break;
			case 'x':
			case 'X':
				bIsShoot = true;
				break;
			default:
				break;
			}
		}

		if (clock() - prevTime > TIME_INTERVAL)
		{
			enemyMove = true;
			prevTime = clock();
		}

		gameManager.Update(bIsLeftKeyPressed, bIsRightKeyPressed, enemyMove, bIsShoot);
		gameManager.Render();
		gameManager.Wait();
	}

	return 0;
}