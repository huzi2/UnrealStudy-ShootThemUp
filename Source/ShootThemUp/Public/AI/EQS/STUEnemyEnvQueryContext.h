// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "STUEnemyEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

private:
	USTUEnemyEnvQueryContext();

private:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const final;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyActorKeyName;
};
