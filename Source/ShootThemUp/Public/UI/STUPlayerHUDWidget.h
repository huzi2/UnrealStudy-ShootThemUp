// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual bool Initialize() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

private:
	void OnHealthChanged(float Health, float HealthDelta);

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
};
