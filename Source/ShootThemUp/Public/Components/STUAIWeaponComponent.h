// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/STUWeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
	GENERATED_BODY()

private:
	virtual void StartFire() final;
	virtual void NextWeapon() final;
};
