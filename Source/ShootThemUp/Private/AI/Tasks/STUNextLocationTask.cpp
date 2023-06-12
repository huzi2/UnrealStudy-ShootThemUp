// Shoot Them Up Game. All Rightes Reserved.


#include "AI/Tasks/STUNextLocationTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
	: Radius(1000.f)
	, bSelfCenter(true)
{
	NodeName = TEXT("NextLocation");
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NavLocation;
	FVector Location = Pawn->GetActorLocation();
	if (!bSelfCenter)
	{
		AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if (!CenterActor)
		{
			return EBTNodeResult::Failed;
		}
		Location = CenterActor->GetActorLocation();
	}

	bool bFount = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if (!bFount)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
