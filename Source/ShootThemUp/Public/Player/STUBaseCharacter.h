// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	virtual void BeginPlay() override;
	virtual void TurnOff() override;
	virtual void Reset() override;

protected:
	virtual void OnDeath();
	virtual void OnHealthChanged(const float Health, const float HealthDelta);

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

public:
	void SetPlayerColor(const FLinearColor& Color);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FName MeterialColorName;
};
