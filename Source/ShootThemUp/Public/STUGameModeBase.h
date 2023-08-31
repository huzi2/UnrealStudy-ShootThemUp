// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	ASTUGameModeBase();

private:
	virtual void StartPlay() final;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) final;
	virtual bool ClearPause() final;

public:
	void Killed(AController* KillerController, AController* VictimController);
	const FGameData& GetGameData() const { return GameData; }
	FORCEINLINE int32 GetCurrentRoundNum() const { return CurrentRound; }
	FORCEINLINE int32 GetRoundSecondsRemaining() const { return RoundCountDown; }
	void RespawnRequest(AController* Controller);

private:
	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(const int32 TeamID) const;
	void SetPlayerColor(AController* Controller);
	void LogPlayerInfo();
	void StartRespawn(AController* Controller);
	void GameOver();
	void SetMatchState(const ESTUMatchState State);
	void StopAllFire();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;
	
public:
	FOnMatchStateChangedSignature OnMatchStateChangedSignature;

private:
	int32 CurrentRound;
	int32 RoundCountDown;
	FTimerHandle GamdRoundTimerHandle;
	ESTUMatchState MatchState;
};
