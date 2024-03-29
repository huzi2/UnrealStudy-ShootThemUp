// Shoot Them Up Game. All Rightes Reserved.


#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
	: CurrentRound(1)
	, RoundCountDown(0)
	, MatchState(ESTUMatchState::WatingToStart)
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool bPauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (bPauseSet)
	{
		StopAllFire();
		SetMatchState(ESTUMatchState::Pause);
	}
	return bPauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
	const bool bPauseCleared = Super::ClearPause();

	if (bPauseCleared)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}
	return bPauseCleared;
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	ASTUPlayerState* KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
	ASTUPlayerState* VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}

	StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ASTUGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	const int32 Num = GameData.PlayersNum - 1;
	for (int32 i = 0; i < Num; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAIController* STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);
	}
}

void ASTUGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GamdRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
	//UE_LOG(LogSTUGameModeBase, Display, TEXT("Time : %i / Round : %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GamdRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	int32 TeamID = 1;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
			if (PlayerState)
			{
				PlayerState->SetTeamID(TeamID);
				PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
				PlayerState->SetPlayerName(Controller->IsPlayerController() ? TEXT("Player") : TEXT("Bot"));
				SetPlayerColor(Controller);

				TeamID = TeamID == 1 ? 2 : 1;
			}
		}
	}
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(const int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}

	UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id : %i, set to default : %s"), TeamID, *GameData.DefaultTeamColor.ToString());
	return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;

	ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if (!Character) return;

	ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
			if (PlayerState)
			{
				PlayerState->LogInfo();
			}
		}
	}
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
	const bool bRespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!bRespawnAvailable) return;

	USTURespawnComponent* RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver()
{
	UE_LOG(LogSTUGameModeBase, Display, TEXT("---------- GAME OVER ----------"));
	LogPlayerInfo();

	for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(const ESTUMatchState State)
{
	if (MatchState == State) return;

	MatchState = State;
	OnMatchStateChangedSignature.Broadcast(MatchState);
}

void ASTUGameModeBase::StopAllFire()
{
	for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
	{
		USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
		if (WeaponComponent)
		{
			WeaponComponent->StopFire();
			WeaponComponent->Zoom(false);
		}
	}
}
