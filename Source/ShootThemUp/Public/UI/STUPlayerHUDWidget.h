// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
private:
	USTUPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

private:
	virtual void NativeOnInitialized() final;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

private:
	void OnHealthChanged(const float Health, const float HealthDelta);
	void OnNewPawn(APawn* NewPawn);
	void UpdateHealthBar();

protected:
	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	FString FormatBullets(const int32 BulletsNum) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor GoodColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor BadColor;
};
