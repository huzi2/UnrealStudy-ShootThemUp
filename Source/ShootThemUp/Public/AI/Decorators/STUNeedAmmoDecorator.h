// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUNeedAmmoDecorator.generated.h"

class ASTUBaseWeapon;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	USTUNeedAmmoDecorator();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const final;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ASTUBaseWeapon> WeaponType;
};
