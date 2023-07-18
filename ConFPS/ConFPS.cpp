#include <iostream>
#include <Windows.h>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;

int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	
	wstring map;
	map += L'###############';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'#.............#';
	map += L'###############';

	//Game Loop
	while (true) {
		for (int x = 0; x < nScreenWidth; x++) {
			//For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
		}

		screen[nScreenHeight * nScreenWidth - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0,0 }, &dwBytesWritten);
	}

	return EXIT_SUCCESS;
}
