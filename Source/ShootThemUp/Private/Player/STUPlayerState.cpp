// Shoot Them Up Game. All Rightes Reserved.


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

ASTUPlayerState::ASTUPlayerState()
	: KillsNum(0)
	, DeathsNum(0)
{
}

void ASTUPlayerState::LogInfo()
{
	UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID : %i, Kills : %i, Deaths : %i"), TeamID, KillsNum, DeathsNum);
}
