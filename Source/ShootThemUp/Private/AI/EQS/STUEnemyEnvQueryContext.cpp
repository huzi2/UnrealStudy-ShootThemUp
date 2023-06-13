// Shoot Them Up Game. All Rightes Reserved.


#include "AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

USTUEnemyEnvQueryContext::USTUEnemyEnvQueryContext()
	: EnemyActorKeyName(TEXT("EnemyActor"))
{
}

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner)
	{
		return;
	}

	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
	if (!Blackboard)
	{
		return;
	}

	AActor* ContextActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName));
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor);
}
