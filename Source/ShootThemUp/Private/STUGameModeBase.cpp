// Shoot Them Up Game. All Rightes Reserved.


#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
	: CurrentRound(1)
	, RoundCountDown(0)
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
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
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
}

void ASTUGameModeBase::SpawnBots()
{
	if (!GetWorld())
	{
		return;
	}

	int32 Num = GameData.PlayersNum - 1;
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
	UE_LOG(LogSTUGameModeBase, Display, TEXT("Time : %i / Round : %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

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
			UE_LOG(LogSTUGameModeBase, Display, TEXT("---------- GAME OVER ----------"));
			LogPlayerInfo();
		}
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if (!GetWorld())
	{
		return;
	}

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
	if (!GetWorld())
	{
		return;
	}

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
				SetPlayerColor(Controller);

				TeamID = TeamID == 1 ? 2 : 1;
			}
		}
	}
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
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
	if (!Controller)
	{
		return;
	}

	ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if (!Character)
	{
		return;
	}

	ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if (!PlayerState)
	{
		return;
	}

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
	if (!GetWorld())
	{
		return;
	}

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
