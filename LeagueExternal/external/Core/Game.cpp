#include "Game.h"

Game::Game()
{
	hGameWindow = FindWindow(NULL, "League of Legends (TM) Client");
	if (!hGameWindow)
		exit(1);



	timestamp1 = std::chrono::system_clock::now();
	timestamp2 = std::chrono::system_clock::now();

	enemiesList.reserve(5);
	p_enemiesList.reserve(5);

	minionsList.reserve(10);
	printf("Game object created\n");
}

Game::~Game() {
	printf("Game object destroyed\n");
}

void Game::Update()
{
	timestamp2 = std::chrono::system_clock::now();
	elapsedTime = timestamp2 - timestamp1;
	timestamp1 = timestamp2;
	fElapsedTime = elapsedTime.count();
	gameTime += fElapsedTime;


	WindowCapture();
}


void Game::WindowCapture()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int boxWidth = screenWidth;
	int boxHeight = screenHeight;
	int boxX = (screenWidth - boxWidth) / 2;
	int boxY = (screenHeight - boxHeight) / 2;

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);

	SelectObject(hdcMemDC, hbmScreen);

	BitBlt(hdcMemDC, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

	GetObject(hbmScreen, sizeof(BITMAP), &GameImage);

	// Create an array to store pixel data
	pixelData = new BYTE[GameImage.bmWidth * GameImage.bmHeight * 4]; // Assuming 32-bit RGBA format

	// Copy pixel data from memory DC to array
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = GameImage.bmWidth;
	bi.biHeight = -GameImage.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32; // 32-bit RGBA format
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	GetDIBits(hdcScreen, hbmScreen, 0, GameImage.bmHeight, pixelData, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Swap Enemies List
	if (!enemiesList.empty()) {
		p_enemiesList.swap(enemiesList);
		enemiesList.clear();
	}

	int ReadY = boxY;
	int ReadX = boxX;
	int EnemiesCount = 0;
	while (ReadY < boxY + boxHeight) {
		while (ReadX < boxX + boxWidth) {
			BYTE* pixel = pixelData + ((ReadY * GameImage.bmWidth + ReadX) * 4);

			if (pixel[2] == 154 && pixel[1] == 38 && pixel[0] == 27) {
				vector2 origin = { (float)ReadX + 50, (float)ReadY + 100 };

				enemiesList.emplace_back(EnemiesCount, origin);
				++EnemiesCount;

				//Avoid Overlap
				ReadY += 20;
				ReadX += 125;
			}

			if (pixel[2] == 101 && pixel[1] == 216 && pixel[0] == 93) {
				vector2 origin = { (float)ReadX+50, (float)ReadY+100 };
				b_localPlayer = { origin };
				localPlayer = &b_localPlayer;

				//Avoid Overlap
				ReadY += 20;
				ReadX += 125;
			}
			++ReadX;
		}
		++ReadY;
		ReadX = boxX;
	}

	// Cleanup
	delete[] pixelData;
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	//localPlayer = nullptr;
}

