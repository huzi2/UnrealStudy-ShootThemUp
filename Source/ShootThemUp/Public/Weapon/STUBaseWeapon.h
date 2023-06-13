// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class UNiagaraComponent;

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
	bool TryToAddAmmo(int32 ClipsAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;

protected:
	bool GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd);
	APlayerController* GetPlayerController() const;
	void DecreaseAmmo();
	bool IsClipEmpty() const;
	void LogAmmo();
	UNiagaraComponent* SpawnMuzzleFX();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* MuzzleFX;

public:
	FOnClipEmptySiganature OnClipEmpty;

private:
	FAmmoData CurrentAmmo;
};
