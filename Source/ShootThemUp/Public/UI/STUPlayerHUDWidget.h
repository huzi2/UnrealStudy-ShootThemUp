// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class USTUWeaponComponent;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const;

	UFUNCTION(BlueprintPure, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const;

private:
	USTUWeaponComponent* GetWeaponComponent() const;
};
