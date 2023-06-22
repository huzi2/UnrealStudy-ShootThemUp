// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
private:
	ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

private:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void OnDeath() final;

private:
	virtual bool IsRunning() const final;

private:
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void TurnAround(const FInputActionValue& Value);
	void OnStartRunning();
	void OnStopRunning();
	void CheckCameraOverlap();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveForwardInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveRightInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookUpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* TurnAroundInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RunInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FireInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* NextWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ReloadInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ZoomInputAction;

private:
	bool bWantsToRun;
	bool bIsMovingForward;
};
