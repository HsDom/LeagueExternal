#include "Utlis.h"

vector2 Utils::MousePos()
{
	POINT pCursorPos;
	GetCursorPos(&pCursorPos);
	ScreenToClient(hGameWindow, &pCursorPos);
	return vector2{ (float)pCursorPos.x, (float)pCursorPos.y };
}

void Utils::MouseMove(vector2 pos)
{
	vector2 mouse = MousePos();
	INPUT in = { 0 };

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_MOVE;
	in.mi.dx = pos.x - mouse.x;
	in.mi.dy = pos.y - mouse.y;

	SendInput(1, &in, sizeof(in));
}

void Utils::MouseMoveRelative(int x, int y)
{
	INPUT in = { 0 };
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_MOVE;
	in.mi.dx = x;
	in.mi.dy = y;
	SendInput(1, &in, sizeof(in));
}

void Utils::MouseMoveSmooth(vector2 pos, int smoothing, int delay)
{
	vector2 delta = pos - MousePos();

	int x = static_cast<int>(delta.x);
	int y = static_cast<int>(delta.y);

	int _x = 0, _y = 0, _t = 0;

	for (int i = 1; i <= smoothing; i++) {
		int xi = i * x / smoothing;
		int yi = i * y / smoothing;
		int ti = i * delay / smoothing;
		MouseMoveRelative(xi - _x, yi - _y);
		std::this_thread::sleep_for(std::chrono::milliseconds(ti - _t));
		_x = xi; _y = yi; _t = ti;
	}
}

void Utils::MouseRightClick(vector2 pos)
{
	INPUT in = { 0 };

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));

	ZeroMemory(&in, sizeof(in));

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));
}

void Utils::MouseLeftClick(vector2 pos)
{
	INPUT in = { 0 };

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));

	ZeroMemory(&in, sizeof(in));

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));
}


void Utils::KeyboardPressKey(char key)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = key;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	// Simulate key release
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}