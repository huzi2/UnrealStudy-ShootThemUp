// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void StartFire();
	virtual void StopFire();

protected:
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const;

public:
	void ChangeClip();
	bool CanReload() const;
	FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetAmmoData() const { return CurrentAmmo; }

protected:
	bool GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd);
	APlayerController* GetPlayerController() const;
	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void LogAmmo();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

public:
	FOnClipEmptySiganature OnClipEmpty;

private:
	FAmmoData CurrentAmmo;
};
