#include "menu.h"


DiscordOverlay::DiscordOverlay(Game* game)
{
	DiscordOverlayWindow = FindWindowA(nullptr, "League Of Legends (TM) Client");
	if (!DiscordOverlayWindow) {
		printf("[!] Target window not found\n");
	}

	// Get target process id
	GetWindowThreadProcessId(DiscordOverlayWindow, reinterpret_cast<LPDWORD>(&targetProcessId));
	processInfo.ProcessId = targetProcessId;

	// Connect to the target process
	if (!ConnectToProcess()) {
		printf("[!] Failed to connect to the target process\n");
	}

	mainHeader = processInfo.MappedAddress;

	// Create device
	if (!CreateDevice()) {
		printf("[!] Failed to create device\n");
	}

	if (!Initialize()) {
		printf("[!] Failed to initialize\n");
	}

	this->game = game;

	printf("DiscordOverlay created\n");

}

void DiscordOverlay::RenderThread() {
	while (Runder) {
		this->RenderLoop();
	}
}



DiscordOverlay::~DiscordOverlay()
{
	// On class destroy
	printf("DiscordOverlay destroyed\n");
}

void DiscordOverlay::CleanupDevice() {
	CleanupRenderTarget();
	if (s_SwapChain) { s_SwapChain->Release(); s_SwapChain = nullptr; }
	if (s_DeviceContext) { s_DeviceContext->Release(); s_DeviceContext = nullptr; }
	if (s_Device) { s_Device->Release(); s_Device = nullptr; }
}
void DiscordOverlay::CleanupRenderTarget() {
	if (s_RenderTargetView) { s_RenderTargetView->Release(); s_RenderTargetView = nullptr; }
}

void DiscordOverlay::CreateRenderTarget() {
	ID3D11Texture2D* pBackBuffer;
	s_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	s_Device->CreateRenderTargetView(pBackBuffer, nullptr, &s_RenderTargetView);
	pBackBuffer->Release();

}

bool DiscordOverlay::CreateDevice() {
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = DiscordOverlayWindow;// DiscordOverlayWindow
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &s_SwapChain, &s_Device, &featureLevel, &s_DeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &s_SwapChain, &s_Device, &featureLevel, &s_DeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget(); // Create the depth and stencil buffers
	return true;
}

void DiscordOverlay::RenderLoop() {
	static bool done = false;
	// Poll and handle messages (inputs, window resize, etc.)
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			done = true;
	}

	ImGuiIO& io = ImGui::GetIO();

	POINT p{};
	GetCursorPos(&p);
	io.MousePos = ImVec2(static_cast<float>(p.x), static_cast<float>(p.y));

	io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

	// Handle window resize (we don't resize directly in the WM_SIZE handler)
	if (s_WindowWidth != 0 && s_WindowHeight != 0) {
		CleanupRenderTarget();
		s_SwapChain->ResizeBuffers(0, s_WindowWidth, s_WindowHeight, DXGI_FORMAT_UNKNOWN, 0);
		s_WindowWidth = s_WindowHeight = 0;
		CreateRenderTarget();
	}

	// keyboard input
	if (GetAsyncKeyState(VK_INSERT) & 1)
		ShowMenu = !ShowMenu;

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ShowMenu)
		MenuRender();

	Drawings(game);



	// Rendering
	ImGui::Render();
	const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	s_DeviceContext->OMSetRenderTargets(1, &s_RenderTargetView, nullptr);
	s_DeviceContext->ClearRenderTargetView(s_RenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Send frame to Discord overlay hook
	SendBuffer();

	s_SwapChain->Present(1, 0); // Present with vsync
	//s_SwapChain->Present(0, 0); // Present without vsync
}

void DiscordOverlay::SendBuffer() {
	ID3D11Device* pDevice;
	s_SwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));

	ID3D11DeviceContext* pContext;
	pDevice->GetImmediateContext(&pContext);

	ID3D11Texture2D* pBackBuffer;
	s_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	D3D11_TEXTURE2D_DESC txtDesc;
	pBackBuffer->GetDesc(&txtDesc);
	txtDesc.Usage = D3D11_USAGE_STAGING;
	txtDesc.BindFlags = 0;
	txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Texture2D* pBackBufferStaging;
	HRESULT status = pDevice->CreateTexture2D(&txtDesc, nullptr, &pBackBufferStaging);
	pContext->CopyResource(pBackBufferStaging, pBackBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	status = s_DeviceContext->Map(pBackBufferStaging, 0, D3D11_MAP_READ, 0, &mappedResource);

	unsigned int bufferSize = 1080 * mappedResource.RowPitch;
	memcpy(mainHeader->Buffer, mappedResource.pData, bufferSize);

	mainHeader->FrameCount++;

	s_DeviceContext->Unmap(pBackBufferStaging, 0);

	pBackBufferStaging->Release();
	pBackBuffer->Release();
}


bool DiscordOverlay::ConnectToProcess() {
	std::string mappedFilename = "DiscordOverlay_Framebuffer_Memory_" + std::to_string(processInfo.ProcessId);
	processInfo.File = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, mappedFilename.c_str());
	if (!processInfo.File || processInfo.File == INVALID_HANDLE_VALUE)
		return false;

	processInfo.MappedAddress = static_cast<Header*>(MapViewOfFile(processInfo.File, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	return processInfo.MappedAddress;
}


bool DiscordOverlay::Initialize(){
	// Show the window
	::ShowWindow(DiscordOverlayWindow, SW_SHOWDEFAULT);
	::UpdateWindow(DiscordOverlayWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// No ini file
	io.IniFilename = nullptr;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(DiscordOverlayWindow);
	ImGui_ImplDX11_Init(s_Device, s_DeviceContext);


	return true;
}


void DiscordOverlay::MenuRender() {
	ImGui::Begin("Hala OrbWalker WadBot Private", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(500, 400));
	ImGui::Checkbox("Example", &Settings::Example);
	ImGui::End();
}
