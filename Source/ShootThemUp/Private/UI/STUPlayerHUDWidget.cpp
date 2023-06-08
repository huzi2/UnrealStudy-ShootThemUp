// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	APawn* Player = GetOwningPlayerPawn();
	if (!Player)
	{
		return 0.f;
	}

	UActorComponent* Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
	USTUHealthComponent* HealthComponent = Cast<USTUHealthComponent>(Component);
	if (!HealthComponent)
	{
		return 0.f;
	}

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& OutUIData) const
{
	USTUWeaponComponent* WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->GetCurrentWeaponUIData(OutUIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& OutAmmoData) const
{
	USTUWeaponComponent* WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->GetCurrentWeaponAmmoData(OutAmmoData);
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
	APawn* Player = GetOwningPlayerPawn();
	if (!Player)
	{
		return nullptr;
	}

	UActorComponent* Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
	USTUWeaponComponent* WeaponComponent = Cast<USTUWeaponComponent>(Component);
	return WeaponComponent;
}