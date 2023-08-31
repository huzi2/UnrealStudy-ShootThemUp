// Shoot Them Up Game. All Rightes Reserved.


#include "AI/Services/STUFireBTService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireBTService::USTUFireBTService()
{
	NodeName = TEXT("Fire");
}

void USTUFireBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	USTUWeaponComponent* WeanponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (!WeanponComponent) return;

	UObject* HasAim = Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	HasAim ? WeanponComponent->StartFire() : WeanponComponent->StopFire();
}
