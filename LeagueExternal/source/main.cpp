#include <Windows.h>
#include <iostream>
#include <thread>
#include "../external/Core/Game.h"
#include "../external/menu.h"
#include "../external/Core/Drawing.h"



void Drawings(Game* game)
{
	auto DrawList = ImGui::GetBackgroundDrawList();
	auto localPlayer = game->localPlayer;
	if (localPlayer == nullptr)
		return;

	// Draw a Dot in the center of the player
	DrawList->AddCircleFilled(ImVec2(localPlayer->origin.x, localPlayer->origin.y), 2, ImColor(255, 0, 0));

	// Draw AA Range
	DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y+25), localPlayer->vScreenAARange.x, localPlayer->vScreenAARange.y, 50, 1, ImColor(255, 0, 0));
	// Draw W Range
	if (localPlayer->bWAbility)
		DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y+95), localPlayer->vWRange.x, localPlayer->vWRange.y, 50, 1, ImColor(0, 255, 0));
}

static Enemy* enemy = nullptr;
static bool CanAttack = true;
int main()
{
	// Initialize game 
	Game game;

	// Initialize Discord Overlay
	DiscordOverlay overlay(&game);
	std::thread renderThread(&DiscordOverlay::RenderThread, &overlay);

	// Main loop
	while (!GetAsyncKeyState(VK_HOME))
	{
		game.Update();
		if (game.localPlayer == nullptr)
			continue;

		// Orbwalker
		if (OrbWalker::Enabled)
		{
			enemy = game.GetClosestEnemy(game.localPlayer->origin);
			if (enemy == nullptr)
				continue;

			if (game.localPlayer->IsInside(*enemy,game.localPlayer->vWRange))
				printf("Attacking\n");
			else
				printf("Moving\n");

		}
	}


    return 0;
}
