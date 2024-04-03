#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "../Core/SDK/structs.h"
#include "../Core/Game.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>


namespace Settings {
	extern bool Example;
}

class DiscordOverlay {
public:
	DiscordOverlay(Game* game);
	~DiscordOverlay();
public:
	Game* game;
	void RenderThread();
	void RenderLoop();
private:
	typedef struct _Header {
		UINT Magic;
		UINT FrameCount;
		UINT NoClue;
		UINT Width;
		UINT Height;
		BYTE Buffer[];
	} Header;

	typedef struct _ConnectedProcessInfo {
		UINT ProcessId;
		HANDLE File;
		Header* MappedAddress;
	} ConnectedProcessInfo;
private:
	bool Runder = true;
	bool ShowMenu;
	HWND DiscordOverlayWindow;
	UINT targetProcessId;

	ConnectedProcessInfo processInfo;
	Header* mainHeader;

	ID3D11Device* s_Device = nullptr;
	ID3D11DeviceContext* s_DeviceContext = nullptr;
	IDXGISwapChain* s_SwapChain = nullptr;
	UINT s_WindowWidth = 0, s_WindowHeight = 0;
	ID3D11RenderTargetView* s_RenderTargetView = nullptr;
private:
	void CleanupDevice();
	void CleanupRenderTarget();
	void CreateRenderTarget();
	bool CreateDevice();
	bool Initialize();
	bool ConnectToProcess();
	void SendBuffer();
	void MenuRender();
};

void Drawings(Game* game);