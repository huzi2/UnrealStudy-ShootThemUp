// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ASTUPlayerController();

private:
	virtual void BeginPlay() final;
	virtual void OnPossess(APawn* InPawn) final;
	virtual void SetupInputComponent() final;

private:
	void OnPauseGame();
	void OnMatchStateChanged(const ESTUMatchState State);
	void OnMuteSound();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTURespawnComponent* RespawnComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* PauseGameInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MuteInputAction;
};
