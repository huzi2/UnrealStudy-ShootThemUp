// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	USTUGameInstance();

public:
	FName GetStartupLevelName() const { return StartupLevelName; }
	FName GetMenuLevelName() const { return MenuLevelName; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartupLevelName;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName;
};
