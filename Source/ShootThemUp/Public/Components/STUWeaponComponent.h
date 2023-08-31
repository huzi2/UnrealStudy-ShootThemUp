// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	USTUWeaponComponent();

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void StartFire();
	virtual void NextWeapon();

public:
	void StopFire();
	void Reload();
	bool GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const;
	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, const int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) const;
	void Zoom(const bool bEnabled);

protected:
	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(const int32 WeaponIndex);

private:
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent); 
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
	bool CanReload() const;
	void OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();

protected:
	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage;

protected:
	int32 CurrentWeaponIndex;

private:
	bool EquipAnimInProgress;
	bool ReloadAnimInProgress;
};
