#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>
#include <future>
#include "SDK/Entities.h"
#include "SDK/requests.h"
#include "SDK/Utlis.h"
#include "../Settings.h"

class Game
{
public:
	Game();
	~Game();

public:
	LocalPlayer* localPlayer;
	std::vector<Enemy> enemies;
	std::vector<Minion> minions;
	float gameTime;
	Utils utils;
public:
	void Update();
	Enemy* GetClosestEnemy(vector2 ref);
private:
	HWND hGameWindow;
	BITMAP GameImage;
	BYTE* pixelData;

	std::chrono::system_clock::time_point timestamp1, timestamp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;

	LocalPlayer b_localPlayer;
	std::vector<Enemy> p_enemies;

	Enemy closestEnemy;

	std::string sAllGameData;
	std::future<void> f;

	float fAspectRatio, w, h;

private:
	void WindowCapture();
	void GetLocalPlayerData();
	void GetEnemiesData();
};

