// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	AActor* GetClosetEnemy() const;
};
