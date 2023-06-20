// Shoot Them Up Game. All Rightes Reserved.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

void USTUMenuWidget::OnStartGame()
{
	USTUGameInstance* GameInstance = GetSTUGameInstance();
	if (!GameInstance)
	{
		return;
	}

	UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
	USTUGameInstance* GameInstance = GetSTUGameInstance();
	if (!GameInstance)
	{
		return;
	}

	checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

	if (!LevelItemsBox)
	{
		return;
	}

	LevelItemsBox->ClearChildren();

	for (const FLevelData& LevelData : GameInstance->GetLevelsData())
	{
		USTULevelItemWidget* LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget)
		{
			continue;
		}

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (GameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(GameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(GameInstance->GetStartupLevel());
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	USTUGameInstance* GameInstance = GetSTUGameInstance();
	if (!GameInstance)
	{
		return;
	}

	GameInstance->SetStartupLevel(Data);

	for (USTULevelItemWidget* LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const bool bIsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(bIsSelected);
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld())
	{
		return nullptr;
	}
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}
