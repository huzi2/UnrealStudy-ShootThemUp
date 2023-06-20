// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"

class ASTUGameModeBase;
class ASTUPlayerState;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetRoundSecondsRemaining() const;

private:
	ASTUGameModeBase* GetSTUGameMode() const;
	ASTUPlayerState* GetSTUPlayserState() const;
};
