// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;
};
