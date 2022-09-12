#include "vardisplay.h"
#include "game.h"

unsigned int color = 0xFFFFFF00;
float offset_x = -1.5;
float offset_y = 0.75;
float font_size = 0.05;

void VarDisplay::Draw()
{

	
	int i = 0;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::XPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::YPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::ZPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::XAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::YAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::ZAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::ScarfCharge).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	Render::AddText(Game::Render, std::to_string(Game::LocalDude::ScarfMax).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
}
