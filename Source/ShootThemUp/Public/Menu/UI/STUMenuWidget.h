// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;
class USoundCue;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

private:
	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	USTUGameInstance* GetSTUGameInstance() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;

private:
	UPROPERTY()
	TArray<USTULevelItemWidget*> LevelItemWidgets;
};
