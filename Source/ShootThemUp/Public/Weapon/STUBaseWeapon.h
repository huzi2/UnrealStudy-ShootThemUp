// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class UNiagaraComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	ASTUBaseWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartFire() {}
	virtual void StopFire() {}
	virtual void Zoom(const bool bEnabled) {}

protected:
	virtual void MakeShot() {}
	virtual bool GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const;

public:
	void ChangeClip();
	bool CanReload() const;
	const FWeaponUIData& GetUIData() const { return UIData; }
	const FAmmoData& GetAmmoData() const { return CurrentAmmo; }
	bool TryToAddAmmo(const int32 ClipsAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;

protected:
	bool GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd);
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireSound;

public:
	FOnClipEmptySiganature OnClipEmpty;

private:
	FAmmoData CurrentAmmo;
};
