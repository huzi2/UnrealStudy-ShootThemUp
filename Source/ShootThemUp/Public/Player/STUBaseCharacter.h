// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;
struct FInputActionValue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void OnDeath();

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

private:
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void TurnAround(const FInputActionValue& Value);
	void OnStartRunning();
	void OnStopRunning();
	void OnHealthChanged(float Health, float HealthDelta);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage;

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

private:
	bool bWantsToRun;
	bool bIsMovingForward;
};
