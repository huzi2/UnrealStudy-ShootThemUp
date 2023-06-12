// Shoot Them Up Game. All Rightes Reserved.


#include "AI/Services/STUFindEnemyService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = TEXT("FindEnemy");
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	USTUAIPerceptionComponent* PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
	if (!PerceptionComponent)
	{
		return;
	}

	Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosetEnemy());
}
