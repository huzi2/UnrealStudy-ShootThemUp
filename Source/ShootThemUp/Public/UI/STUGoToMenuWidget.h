// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() final;

private:
	UFUNCTION()
	void OnToToMenu();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* GoToMenuButton;
};
