#include "Game.h"
#include "Player.h"

int main()
{
	Game::GetInstance()->Init();
    Game::GetInstance()->Run();
}
