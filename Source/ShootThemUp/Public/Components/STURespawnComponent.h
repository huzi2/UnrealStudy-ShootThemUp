// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();

public:
	int32 GetRespawnCountDown() const { return RespawnCountDown; }

	void Respawn(int32 RespawnTime);
	bool IsRespawnInProgress() const;

private:
	void RespawnTimerUpdate();

private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountDown;
};
