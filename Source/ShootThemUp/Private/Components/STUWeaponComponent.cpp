// Shoot Them Up Game. All Rightes Reserved.


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/STUEquipFinishedAnimNotify.h"
#include "Animation/STUReloadFinishedAnimNotify.h"
#include "Animation/AnimUtil.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
	: CurrentWeapon(nullptr)
	, WeaponEquipSocketName(TEXT("WeaponSocket"))
	, WeaponArmorySocketName(TEXT("ArmorySocket"))
	, CurrentReloadAnimMontage(nullptr)
	, CurrentWeaponIndex(0)
	, EquipAnimInProgress(false)
	, ReloadAnimInProgress(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
	if (!CanFire())
	{
		return;
	}

	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip())
	{
		return;
	}

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (ASTUBaseWeapon* Weapon : Weapons)
	{
		if (Weapon)
		{
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Weapon->Destroy();
		}
	}

	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const
{
	if (CurrentWeapon)
	{
		OutUIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const
{
	if (CurrentWeapon)
	{
		OutAmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	if (!WeaponType)
	{
		return false;
	}

	for (ASTUBaseWeapon* Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) const
{
	if (!WeaponType)
	{
		return false;
	}

	for (ASTUBaseWeapon* Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= WeaponData.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invailed weapon index"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];

	const FWeaponData* CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
		{
			return Data.WeaponClass == CurrentWeapon->GetClass();
		});

	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld())
	{
		return;
	}

	for (const FWeaponData& OneWeaponData : WeaponData)
	{
		ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon)
		{
			continue;
		}

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent)
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
	USTUEquipFinishedAnimNotify* EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (const FWeaponData& OneWeaponData : WeaponData)
	{
		USTUReloadFinishedAnimNotify* ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (ReloadFinishedNotify)
		{
			ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		}
		else
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComponent)
	{
		return;
	}

	EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComponent)
	{
		return;
	}

	ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon)
	{
		return;
	}

	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (ASTUBaseWeapon* Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void USTUWeaponComponent::ChangeClip()
{
	if (!CanReload())
	{
		return;
	}

	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}
