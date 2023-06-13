// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldBeTaken.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()
	
private:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const final;

private:
	UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitialializer);
};
