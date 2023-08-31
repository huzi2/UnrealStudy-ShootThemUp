// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnToToMenu);
	}
}

void USTUGoToMenuWidget::OnToToMenu()
{
	if (!GetWorld()) return;

	USTUGameInstance* GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
}
