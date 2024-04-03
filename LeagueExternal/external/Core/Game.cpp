#include "Game.h"

Game::Game()
{
	hGameWindow = FindWindow(NULL, "League of Legends (TM) Client");
	if (!hGameWindow)
		exit(1);

	timestamp1 = std::chrono::system_clock::now();
	timestamp2 = std::chrono::system_clock::now();

	enemies.reserve(5);
	p_enemies.reserve(5);

	minions.reserve(10);
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

	f = std::async(
		std::launch::async,
		HttpRequestGet,
		"https://127.0.0.1:2999/liveclientdata/allgamedata",
		&sAllGameData
	);


	WindowCapture();
	GetLocalPlayerData();
	GetEnemiesData();
}


void AbilityCooldown(bool& ability, vector2 position, BYTE* pixelData, const BITMAP& bmpScreen) {
	BYTE* pixel = pixelData + (static_cast<int>(position.y) * bmpScreen.bmWidth + static_cast<int>(position.x)) * 4;
	if (pixel[2] > 190 && pixel[1] > 165 && pixel[0] > 100) {
		ability = true;
	}
	else {
		ability = false;
	}
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

	//ScreenBottom
	h = static_cast<float>(GameImage.bmHeight);
	w = static_cast<float>(GameImage.bmWidth);

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
	if (!enemies.empty()) {
		p_enemies.swap(enemies);
		enemies.clear();
	}

	// Clear Minions List
	if (!minions.empty())
		minions.clear();

	vector2 CoolDownR = { 960, 993 };
	vector2 CoolDownW = { 870, 993 };
	vector2 CoolDownQ = { 830, 993 };
	vector2 CoolDownE = { 920, 993 };



	int ReadY = boxY;
	int ReadX = boxX;
	int EnemiesCount = 0;
	while (ReadY < boxY + boxHeight) {
		while (ReadX < boxX + boxWidth) {
			BYTE* pixel = pixelData + ((ReadY * GameImage.bmWidth + ReadX) * 4);

			// Enemy
			if (pixel[2] == 154 && pixel[1] == 38 && pixel[0] == 27) {
				vector2 origin = { (float)ReadX + 50, (float)ReadY + 100 };

				enemies.emplace_back(EnemiesCount, origin);
				++EnemiesCount;

				//Avoid Overlap
				ReadY += 20;
				ReadX += 125;
			}
			
			// Local Player
			if (pixel[2] == 101 && pixel[1] == 216 && pixel[0] == 93) {
				vector2 origin = { (float)ReadX+ 40, (float)ReadY+ 150 };
				b_localPlayer = { origin };

				AbilityCooldown(b_localPlayer.bQAbility, CoolDownQ, pixelData, GameImage);
				AbilityCooldown(b_localPlayer.bWAbility, CoolDownW, pixelData, GameImage);
				AbilityCooldown(b_localPlayer.bEAbility, CoolDownE, pixelData, GameImage);
				AbilityCooldown(b_localPlayer.bRAbility, CoolDownR, pixelData, GameImage);

				localPlayer = &b_localPlayer;

				//Avoid Overlap
				ReadY += 20;
				ReadX += 125;
			}

			// Minion
			if (pixel[2] == 208 && pixel[1] == 94 && pixel[0] == 94) {
				int PixelsAccross = 0;
				while (pixel[2] == 208 && pixel[1] == 94 && pixel[0] == 94) {
					++PixelsAccross;
					pixel = pixelData + ((ReadY * GameImage.bmWidth + ReadX + PixelsAccross) * 4);
				}

				vector2 origin = { (float)ReadX, (float)ReadY  };
				minions.emplace_back(origin, PixelsAccross);

				//Avoid Overlap
				ReadY += 5;
				ReadX += PixelsAccross;
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


void Game::GetLocalPlayerData() {
	if (sAllGameData.empty())
		return;

	json j;

	try {
		j = json::parse(sAllGameData);
	}
	catch (json::parse_error& ex) {
		//std::cout << ex.what() << std::endl;
		//std::cout << ex.id << std::endl;
		return;
	}

	b_localPlayer.stats = j["activePlayer"]["championStats"];

	// bad approx
	float a = h / w;

	float Modifer = 170.0f;
	// Auto Attack Range
	float fScreenAARangeRadius = b_localPlayer.stats.attackRange * (1 - a) + 170.0f;
	b_localPlayer.vScreenAARange.x = fScreenAARangeRadius;
	b_localPlayer.vScreenAARange.y = fScreenAARangeRadius * cos(35.f * 3.1415f / 180.f);

	// Q Range
	float fQRangeRadius = SelectedChampion::QRange * (1 - a) + Modifer;
	b_localPlayer.vQRange.x = fQRangeRadius;
	b_localPlayer.vQRange.y = fQRangeRadius * cos(35.f * 3.1415f / 180.f);

	// W Range
	float fWRangeRadius = SelectedChampion::WRange * (1 - a) + 235.0f;
	b_localPlayer.vWRange.x = fWRangeRadius;
	b_localPlayer.vWRange.y = fWRangeRadius * cos(35.f * 3.1415f / 180.f);

	// E Range
	float fERangeRadius = SelectedChampion::ERange * (1 - a) + Modifer;
	b_localPlayer.vERange.x = fERangeRadius;
	b_localPlayer.vERange.y = fERangeRadius * cos(35.f * 3.1415f / 180.f);

	// R Range
	float fRRangeRadius = SelectedChampion::RRange * (1 - a) + Modifer;
	b_localPlayer.vRRange.x = fRRangeRadius;
	b_localPlayer.vRRange.y = fRRangeRadius * cos(35.f * 3.1415f / 180.f);
}

void Game::GetEnemiesData(){
	int m_size = (p_enemies.size() < enemies.size()) ? p_enemies.size() : enemies.size();

	for (int i = 0; i < m_size; i++)
	{
		enemies[i].direction = enemies[i].origin - p_enemies[i].origin;
		enemies[i].magnitude = sqrt(enemies[i].direction.x * enemies[i].direction.x + enemies[i].direction.y * enemies[i].direction.y);
		enemies[i].speed = enemies[i].magnitude / fElapsedTime;
	}
}

Enemy* Game::GetClosestEnemy(vector2 ref)
{
	float fMaxDist = 9999.0f;

	if (enemies.size() == 0)
		return nullptr;

	for (int i = 0; i < enemies.size(); i++) {

		vector2 vDistance = enemies[i].origin - ref;
		float fDistance = sqrt(vDistance.x * vDistance.x + vDistance.y * vDistance.y);

		if (fDistance < fMaxDist) {
			fMaxDist = fDistance;
			closestEnemy = enemies[i];
		}

	}

	return &closestEnemy;
}