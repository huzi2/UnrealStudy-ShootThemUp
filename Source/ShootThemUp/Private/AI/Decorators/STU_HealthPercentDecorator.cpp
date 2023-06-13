// Shoot Them Up Game. All Rightes Reserved.


#include "AI/Decorators/STU_HealthPercentDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

USTU_HealthPercentDecorator::USTU_HealthPercentDecorator()
	: HealthPercent(0.6f)
{
	NodeName = TEXT("HealthPercent");
}

bool USTU_HealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return false;
	}

	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return false;
	}

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}
