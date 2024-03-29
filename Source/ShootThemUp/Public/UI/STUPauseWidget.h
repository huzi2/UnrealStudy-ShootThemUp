// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() final;

private:
	UFUNCTION()
	void OnClearPause();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;
};
