#pragma once
#include <thread>
#include <chrono>
#include <Windows.h>

#include "structs.h"

class Utils
{
public:
	Utils() {};
	Utils(HWND hwnd) { hGameWindow = hwnd; };

private:
	HWND hGameWindow;

public:
	vector2 MousePos();

	void MouseMove(vector2 pos);

	void MouseMoveRelative(int x, int y);

	void MouseMoveSmooth(vector2 pos, int smoothing, int delay);

	void MouseRightClick(vector2 pos);

	void MouseLeftClick(vector2 pos);

	void KeyboardPressKey(char key);
};

