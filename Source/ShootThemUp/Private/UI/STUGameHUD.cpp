// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

ASTUGameHUD::ASTUGameHUD()
	: CurrentWidget(nullptr)
{
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

	for (auto GameWidgetPair : GameWidgets)
	{
		UUserWidget* GameWidget = GameWidgetPair.Value;
		if (GameWidget)
		{
			GameWidget->AddToViewport();
			GameWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (GetWorld())
	{
		ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChangedSignature.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
		}
	}
}

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.f;
	const float LineThickness = 2.f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChanged(const ESTUMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}

	UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed : %s"), *UEnum::GetValueAsString(State));
}
