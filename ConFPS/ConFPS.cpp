#include <iostream>
#include <Windows.h>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 1.0f;
float fPlayerY = 1.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;
float fDepth = 16.0f;

int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	
	wstring map;
	map += L"################";
	map += L"#.##.........#.#";
	map += L"#.#..####.#..#.#";
	map += L"#.........#..#.#";
	map += L"###########..#.#";
	map += L"#.......#....#.#";
	map += L"#####..##...##.#";
	map += L"#...........#..#";
	map += L"#..#####....#..#";
	map += L"#..#....#####..#";
	map += L"##...###.......#";
	map += L"##.##....####..#";
	map += L"#.....####.....#";
	map += L"#######....#####";
	map += L"#..#....##.....#";
	map += L"################";

	float fAddFov = 2.0;
	
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//Game Loop
	while (true) {

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		//Controls
		//Handle CCW Rotation

		if (GetAsyncKeyState((unsigned short)'J') & 0x8000) fAddFov += 1.0 * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'K') & 0x8000) fAddFov -= 0.5 * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) fPlayerA -= fAddFov * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) fPlayerA += fAddFov * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		if (fPlayerX <= 15.0 && fPlayerX >= 14.0 && fPlayerY >= 14.0 && fPlayerY <= 15.0) { CloseHandle(hConsole); cout << "游戏结束" << endl; 
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;
		cout<<"	________________________________________iii_________________________!!!_" << endl;
		cout<<"	_______________________________________i:::i_______________________!!:!!" << endl;
		cout<<"	________________________________________iii________________________!:::!" << endl;
		cout<<"	___________________________________________________________________!:::!" << endl;
		cout<<"wwwww____________www____________wwwww____iiiiii____nnn__nnnnnnn_____!:::!" << endl;
		cout<<"_w:::w__________w:::w__________w:::w ____i::::i____n::nn:::::::nn___!:::!" << endl;
		cout<<"__w::::w_______w:::::w________w:::w______i:::i____ n:::::::::::nn___!:::!" << endl;
		cout<<"___w::::w_____w:::::::w______w:::w_______i:::i____ nn:::::::::::n___!:::!" << endl;
		cout<<"____w::::w___w::::w::::w___w::::w________i:::i____ n::::nnnn::::n___!:::!" << endl;
		cout<<"_____w::::w_w::::w_w::::w_w::::w_________i:::i____ n:::n____n:::n___!:::!" << endl;
		cout<<"______w::::w::::w___w::::w::::w__________i:::i____ n:::n____n:::n___!!:!!" << endl;
		cout<<"_______w:::::::w_____w:::::::w___________i:::i____ n:::n____n:::n____!!!_" << endl;
		cout<<"________w:::::w_______w:::::w____________i::::i __ n:::n____n:::n________" << endl;
		cout<<"_________w:::w_________w:::w_____________i:::::i__ n:::n____n:::n____!!!_" << endl;
		cout<<"__________w:w___________w:w______________i:::::i__ n:::n____n:::n___!!:!!" << endl;
		cout<<"___________w_____________w_______________iiiiiii__ nnnnn____nnnnn____!!!_" << endl;
		system("pause"); return 0; 																														  
		}
		for (int x = 0; x < nScreenWidth; x++) {
			//For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle); //Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth) {
				
				fDistanceToWall += 0.1f;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				//Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true; //Just srt distance to maximum depth
					fDistanceToWall = fDepth;
				}
				else {
					//Ray is inbounds so test to see if the ray	cell is a wall block
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bHitWall = true;

						vector<pair<float, float>> p; //distance ,dot

						for (int tx = 0; tx < 2; tx++) {
							for (int ty = 0; ty < 2; ty++) {
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}
						}
						//Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						//if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
				
			}
			
			//Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShadeWall = ' ';

			if (fDistanceToWall <= fDepth / 4.0f)        nShadeWall = 0x2588;    //vary close
			else if (fDistanceToWall < fDepth / 3.0f)    nShadeWall = 0x2591;
			else if (fDistanceToWall < fDepth / 2.0f)    nShadeWall = 0x2592;
			else if (fDistanceToWall < fDepth)           nShadeWall = 0x2593;
			else                                         nShadeWall = ' ';       //Too far away

			if (bBoundary)                               nShadeWall = ' ';

			for (int y = 0; y < nScreenHeight; y++) {
				if (y < nCeiling) 
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor) 
					screen[y * nScreenWidth + x] = nShadeWall;
				else {
					//Shade floor based on distance
					short nShadeFloor = ' ';
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)         nShadeFloor = '#';
					else if (b < 0.5)     nShadeFloor = 'x';
					else if (b < 0.75)    nShadeFloor = '.';
					else if (b < 0.9)     nShadeFloor = '-';
					else                  nShadeFloor = ' ';
					screen[y * nScreenWidth + x] = nShadeFloor;
				}
			}

		}

		//Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
		wstring mapScreen;
		//Dispaly Map
		for (int nx = 0; nx < nMapWidth; nx++) {
			for (int ny = 0; ny < nMapHeight; ny++) {
				
				mapScreen = L"################";
				mapScreen += L"#.##.........#.#";
				mapScreen += L"#.#..####.#..#.#";
				mapScreen += L"#.........#..#.#";
				mapScreen += L"###########..#.#";
				mapScreen += L"#.......#....#.#";
				mapScreen += L"#####..##...##.#";
				mapScreen += L"#...........#..#";
				mapScreen += L"#..#####....#..#";
				mapScreen += L"#..#....#####..#";
				mapScreen += L"##...###.......#";
				mapScreen += L"##.##....####..#";
				mapScreen += L"#.....####.....#";
				mapScreen += L"#######....#####";
				mapScreen += L"#..#....##.....#";
				mapScreen += L"################";
				mapScreen[(int)fPlayerY * nMapWidth + (int)fPlayerX] = L'o';
				screen[(ny + 1) * nScreenWidth + nx + 1] = mapScreen[ny * nMapWidth + nx];
			}
		}

		screen[nScreenHeight * nScreenWidth - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0,0 }, &dwBytesWritten);
	}

	return EXIT_SUCCESS;
}
