// Shoot Them Up Game. All Rightes Reserved.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

ASTUHealthPickup::ASTUHealthPickup()
	: HealthAmount(100.f)
{
}

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return false;
	}

	return HealthComponent->TryToAddHealth(HealthAmount);
}
