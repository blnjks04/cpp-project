#include "PlayerInfoView.h"
#include "Player.h"
#include "ConsoleFramework.h"
#include "CharacterView.h"

void PlayerInfoView::DisplayPlayerInfo(Player* player)
{
    CharacterView characterView;

    if (player == nullptr) return;
    PRINT_TEXT(0, 0, "========================================================================================================================");
    PRINT_TEXT(2, 1, "PLAYER INFO");
    PRINT_TEXT(0, 2, "========================================================================================================================");
    PRINT_TEXT(2, 3, "PLAYER");
    PRINT_TEXT(2, 4, std::format("[ {} ]", player->GetName()), "blue");
    PRINT_TEXT(2, 5, std::format("Level: {}  HP: {}/{}  ATK: {}  Gold: {}", player->GetLevel(), player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage(), player->GetGold()), "blue");
    characterView.DisplayPlayer(8, 7);
    characterView.DisplaySword(12, 7);
}