// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

class USoundClass;
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
	FLevelData GetStartupLevel() const { return StartupLevel; }
	void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }
	TArray<FLevelData> GetLevelsData() const { return LevelsData; }
	FName GetMenuLevelName() const { return MenuLevelName; }
	void ToggleVolume();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundClass* MasterSoundClass;

private:
	FLevelData StartupLevel;
};
