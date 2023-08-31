// Shoot Them Up Game. All Rightes Reserved.


#include "Components/STUHealthComponent.h"
#include "STUGameModeBase.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthCompnent, All, All);

USTUHealthComponent::USTUHealthComponent()
	: MaxHealth(100.f)
	, bAutoHeal(true)
	, HealUpdateTime(1.f)
	, HealDelay(3.f)
	, HealModifier(5.f)
	, Health(0.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0.f);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthCompnent, Display, TEXT("On any damage : %f"), Damage);
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	const float FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);

	UE_LOG(LogHealthCompnent, Display, TEXT("On point damage : %f, final damage : %f, bone : %s"), Damage, FinalDamage, *BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthCompnent, Display, TEXT("On radial damage : %f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
}

bool USTUHealthComponent::TryToAddHealth(const float HealthAmount)
{
	if (IsHealthFull() || IsDead()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(const float NewHealth)
{
	const float NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const float HeathDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HeathDelta);
}

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if (!CameraShake || IsDead()) return;

	APawn* Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	APlayerController* Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (!KillerController) return;
	if (!GetWorld()) return;

	ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	APawn* Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	AController* VictimController = Player->Controller;
	if (!VictimController) return;

	GameMode->Killed(KillerController, VictimController);
}

void USTUHealthComponent::ApplyDamage(const float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (bAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();

	ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	ACharacter* Character = Cast<ACharacter>(DamagedActor);
	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.f;

	UPhysicalMaterial* PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!DamageModifiers.Contains(PhysMaterial)) return 1.f;

	return DamageModifiers[PhysMaterial];
}

void USTUHealthComponent::ReportDamageEvent(const float Damage, AController* InstigatedBy)
{
	if (!GetWorld() || !InstigatedBy || !GetOwner() || !InstigatedBy->GetPawn()) return;

	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}
