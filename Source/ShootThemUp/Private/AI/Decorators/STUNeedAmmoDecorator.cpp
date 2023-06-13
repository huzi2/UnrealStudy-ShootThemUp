// Shoot Them Up Game. All Rightes Reserved.


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	NodeName = TEXT("NeedAmmo");
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return false;
	}

	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->NeedAmmo(WeaponType);
}
