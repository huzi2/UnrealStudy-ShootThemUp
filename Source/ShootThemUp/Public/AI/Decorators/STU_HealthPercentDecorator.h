// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STU_HealthPercentDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTU_HealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	USTU_HealthPercentDecorator();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const final;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthPercent;
};
