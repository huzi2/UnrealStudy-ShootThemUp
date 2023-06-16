// Shoot Them Up Game. All Rightes Reserved.


#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
	: MuzzleSocketName(TEXT("MuzzleSocket"))
	, TraceMaxDistance(1500.f)
	, DefaultAmmo({15, 10, false})
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMeshComponent);
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	check(WeaponMeshComponent);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't b less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't b less or equal zero"));

	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::MakeShot()
{
}

bool ASTUBaseWeapon::GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		return false;
	}

	OutTraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	OutTraceEnd = OutTraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Display, TEXT("No more clips"));
			return;
		}
		--CurrentAmmo.Clips;
	}

	CurrentAmmo.Bullets = DefaultAmmo.Bullets;

	//UE_LOG(LogBaseWeapon, Display, TEXT("----- Change Clip -----"));
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
	{
		return false;
	}

	if (IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const int32 NextClipAmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}

	return true;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const
{
	ACharacter* STUCharacter = Cast<ACharacter>(GetOwner());
	if (!STUCharacter)
	{
		return false;
	}

	if (STUCharacter->IsPlayerControlled())
	{
		APlayerController* Controller = STUCharacter->GetController<APlayerController>();
		if (!Controller)
		{
			return false;
		}

		Controller->GetPlayerViewPoint(OutViewLocation, OutViewRotation);
	}
	else
	{
		OutViewLocation = GetMuzzleWorldLocation();
		OutViewRotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
	}
	
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld())
	{
		return;
	}

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	//GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
	GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, CollisionParams);

	if (OutHitResult.GetActor())
		UE_LOG(LogBaseWeapon, Warning, TEXT("test : %s"), *OutHitResult.GetActor()->GetName());
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Clip is empty"));
		return;
	}

	--CurrentAmmo.Bullets;
	//LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = TEXT("Ammo : ") + FString::FromInt(CurrentAmmo.Bullets) + TEXT(" / ");
	AmmoInfo += CurrentAmmo.Infinite ? TEXT("Infinite") : FString::FromInt(CurrentAmmo.Clips);

	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMeshComponent, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}
