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
	FORCEINLINE void SetTeamID(const int32 ID) { TeamID = ID; }
	FORCEINLINE void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }

	FORCEINLINE int32 GetTeamID() const{ return TeamID; }
	FORCEINLINE FLinearColor GetTeamColor() const{ return TeamColor; }
	FORCEINLINE int32 GetKillsNum() const { return KillsNum; }
	FORCEINLINE int32 GetDeathsNum() const { return DeathsNum; }

	FORCEINLINE void AddKill() { ++KillsNum; }
	FORCEINLINE void AddDeath() { ++DeathsNum; }

	void LogInfo();

private:
	int32 TeamID;
	FLinearColor TeamColor;
	int32 KillsNum;
	int32 DeathsNum;
};
