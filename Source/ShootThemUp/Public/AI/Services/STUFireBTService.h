// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFireBTService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUFireBTService : public UBTService
{
	GENERATED_BODY()
	
private:
	USTUFireBTService();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;
};
