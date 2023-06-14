// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	ASTUPlayerState();

public:
	void SetTeamID(int32 ID) { TeamID = ID; }
	void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }

	int32 GetTeamID() const{ return TeamID; }
	FLinearColor GetTeamColor() const{ return TeamColor; }
	int32 GetKillsNum() const { return KillsNum; }
	int32 GetDeathsNum() const { return DeathsNum; }

	void AddKill() { ++KillsNum; }
	void AddDeath() { ++DeathsNum; }

	void LogInfo();

private:
	int32 TeamID;
	FLinearColor TeamColor;
	int32 KillsNum;
	int32 DeathsNum;
};
