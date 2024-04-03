#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>
#include <future>
#include "SDK/Entities.h"

class Game
{
public:
	Game();
	~Game();

public:
	LocalPlayer* localPlayer;
	std::vector<Enemy> enemiesList;
	std::vector<Minion> minionsList;
	float gameTime;

public:
	void Update();
private:
	HWND hGameWindow;
	BITMAP GameImage;
	BYTE* pixelData;

	std::chrono::system_clock::time_point timestamp1, timestamp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;

	LocalPlayer b_localPlayer;
	std::vector<Enemy> p_enemiesList;

	Enemy closestEnemy;

	std::string sAllGameData;
	std::future<void> f;

	float fAspectRatio, w, h;

private:
	void WindowCapture();
};

