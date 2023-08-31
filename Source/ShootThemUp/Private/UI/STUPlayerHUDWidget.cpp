// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"

USTUPlayerHUDWidget::USTUPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PercentColorThreshold(0.3f)
	, GoodColor(FLinearColor::White)
	, BadColor(FLinearColor::Red)
{
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}
}

void USTUPlayerHUDWidget::OnHealthChanged(const float Health, const float HealthDelta)
{
	if (HealthDelta < 0.f)
	{
		OnTakeDamage();

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
	if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
	}

	UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
	if (!HealthProgressBar) return;

	HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent ? HealthComponent->GetHealthPercent() : 0.f;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const
{
	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	return WeaponComponent ? WeaponComponent->GetCurrentWeaponUIData(OutUIData) : false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const
{
	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	return WeaponComponent ? WeaponComponent->GetCurrentWeaponAmmoData(OutAmmoData) : false;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent ? !HealthComponent->IsDead() : false;
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	APlayerController* Controlloer = GetOwningPlayer();
	return Controlloer ? Controlloer->GetStateName() == NAME_Spectating : false;
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
	APlayerController* Controlloer = GetOwningPlayer();
	if (!Controlloer) return 0;

	ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controlloer->PlayerState);
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

FString USTUPlayerHUDWidget::FormatBullets(const int32 BulletsNum) const
{
	constexpr int32 MaxLen = 3;
	constexpr TCHAR PrefixSymbol = '0';

	FString BulletStr = FString::FromInt(BulletsNum);
	const int32 SymbolsNumToAdd = MaxLen - BulletStr.Len();

	if (SymbolsNumToAdd > 0)
	{
		BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
	}
	return BulletStr;
}
