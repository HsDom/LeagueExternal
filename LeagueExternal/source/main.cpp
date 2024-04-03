#include <Windows.h>
#include <iostream>
#include <thread>
#include "../external/Core/Game.h"
#include "../external/menu.h"

void Drawings(Game* game)
{
	auto DrawList = ImGui::GetBackgroundDrawList();
	// Drawings
	if (game->localPlayer != nullptr && Settings::Example)
	{
		vector2 origin = game->localPlayer->origin;
		DrawList->AddCircle(ImVec2(origin.x, origin.y), 10, ImColor(255, 0, 0), 12);
	}

	if (Settings::Example)
	{
		for (int i = 0; i < game->enemiesList.size(); i++)
		{
			vector2 origin = game->enemiesList[i].origin;
			vector2 Prediction = game->enemiesList[i].Predict(0.0f) * 1.15f;
			int id = game->enemiesList[i].id;
			DrawList->AddCircle(ImVec2(origin.x, origin.y), 10, ImColor(0, 255, 0), 12);
			DrawList->AddText(ImVec2(origin.x, origin.y), ImColor(255, 255, 255), std::to_string(id).c_str());
			DrawList->AddLine(ImVec2(origin.x, origin.y), ImVec2(Prediction.x, Prediction.y), ImColor(255, 255, 255), 1);
		}
	}
}

int main()
{
	// Initialize game 
	Game game;

	// Initialize Discord Overlay
	DiscordOverlay overlay(&game);
	std::thread renderThread(&DiscordOverlay::RenderThread, &overlay);

	static Enemy* enemy = nullptr;
	// Main loop
	while (!GetAsyncKeyState(VK_HOME))
	{
		game.Update();
		if (game.localPlayer == nullptr)
			continue;
		//vector2 origin = game.localPlayer->origin;

		for (int i = 0; i < game.enemiesList.size(); i++)
		{
			vector2 origin = game.enemiesList[i].origin;
			int g = game.enemiesList[i].id;
		}
	}


    return 0;
}
