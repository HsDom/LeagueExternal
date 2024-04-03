#pragma once
#include "imgui.h"


void DrawText(ImVec2 position, ImColor color, const char* text, bool outline, bool center) {
	auto DrawList = ImGui::GetBackgroundDrawList();
	if (center) {
		auto textSize = ImGui::CalcTextSize(text);
		position.x -= textSize.x / 2;
		position.y -= textSize.y / 2;
	}

	if (outline) {
		DrawList->AddText(ImVec2(position.x+1, position.y+1), IM_COL32(0, 0, 0, 255), text);
	}
	DrawList->AddText(position, color, text);
}


# define M_PI           3.14159265358979323846 
void DrawEllipse(ImVec2 center, int a, int b, int numSides, int thickness, ImColor color)
{
	float Step = M_PI * 2.0f / numSides;
	int Count = 0;
	for (float t = 0; t < M_PI * 2.0; t += Step) {
		float X1 = a * cos(t) + center.x;
		float Y1 = b * sin(t) + center.y;
		float X2 = a * cos(t + Step) + center.x;
		float Y2 = b * sin(t + Step) + center.y;
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X1, Y1), ImVec2(X2, Y2), color, thickness);
		Count += 2;
	}
}
