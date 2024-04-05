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

	// Auto Attack Range
	DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y + SelectedChampion::AutoAblity::OriginOffset), localPlayer->vScreenAARange.x, localPlayer->vScreenAARange.y, 50, 1, ImColor(255, 0, 0));

	// Q Range
	if (SelectedChampion::QAblity::Range > 0 && localPlayer->bQAbility)
		DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y + SelectedChampion::QAblity::OriginOffset), localPlayer->vScreenQRange.x, localPlayer->vScreenQRange.y, 50, 1, ImColor(0, 255, 0));

	// W Range
	if (SelectedChampion::WAblity::Range > 0 && localPlayer->bWAbility)
		DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y + SelectedChampion::WAblity::OriginOffset), localPlayer->vScreenWRange.x, localPlayer->vScreenWRange.y, 50, 1, ImColor(0,0, 255));

	// E Range
	if (SelectedChampion::EAblity::Range > 0 && localPlayer->bEAbility)
		DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y + SelectedChampion::EAblity::OriginOffset), localPlayer->vScreenERange.x, localPlayer->vScreenERange.y, 50, 1, ImColor(255, 255, 0));

	// R Range
	if (SelectedChampion::RAblity::Range > 0 && localPlayer->bRAbility)
		DrawEllipse(ImVec2(localPlayer->origin.x, localPlayer->origin.y + SelectedChampion::RAblity::OriginOffset), localPlayer->vScreenRRange.x, localPlayer->vScreenRRange.y, 50, 1, ImColor(0, 255, 255));
}

static Enemy* enemy = nullptr;
static bool CanAttack = true;
static vector2 vCursorPos;
static float fDelay = 0.0f;
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
			// Check if Z key is not pressed
			if (!GetAsyncKeyState('Z'))
				continue;

			vCursorPos = game.utils.MousePos();

			enemy = game.GetClosestEnemy(game.localPlayer->origin);
			if (enemy == nullptr)
			{
				Sleep(250);
				game.utils.MouseRightClick(vCursorPos);
				continue;
			}

			
			// Twtich Settings
			// W Ability
			/**/
			if (game.localPlayer->bWAbility && game.localPlayer->IsInside(*enemy, game.localPlayer->vScreenWRange, SelectedChampion::WAblity::OriginOffset))
			{
				game.utils.MouseMove(enemy->origin);
				game.utils.KeyboardPressKey('W');
				Sleep(5);
				game.utils.MouseMove(vCursorPos);
			}

			// E Ability
			if (game.localPlayer->bEAbility && game.localPlayer->IsInside(*enemy, game.localPlayer->vScreenERange, SelectedChampion::EAblity::OriginOffset))
			{
				game.utils.MouseMove(enemy->origin);
				game.utils.KeyboardPressKey('E');
				Sleep(5);
				game.utils.MouseMove(vCursorPos);
			}

			// Auto Attack
			if (CanAttack && game.localPlayer->IsInside(*enemy, game.localPlayer->vScreenAARange, SelectedChampion::AutoAblity::OriginOffset))
			{
				float AttackCooldown = 1000.0f / game.localPlayer->stats.attackSpeed;
				float AttackWindup = AttackCooldown * SelectedChampion::AttackWindup;

				game.utils.MouseMove(enemy->origin);
				game.utils.MouseRightClick(enemy->origin);
				Sleep(AttackWindup);
				game.utils.MouseMove(vCursorPos);
				game.utils.MouseRightClick(vCursorPos);
				CanAttack = false;
				fDelay = (game.gameTime * 1000.0f) + AttackCooldown;
			}

			if (game.gameTime * 1000.0f >= fDelay)
				CanAttack = true;
		}
	}


    return 0;
}
