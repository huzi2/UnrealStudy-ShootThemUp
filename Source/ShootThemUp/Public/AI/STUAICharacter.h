// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
private:
	ASTUAICharacter(const FObjectInitializer& ObjInit);

private:
	virtual void OnDeath() final;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
};
