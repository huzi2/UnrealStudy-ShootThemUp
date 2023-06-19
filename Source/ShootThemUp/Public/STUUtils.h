// Shoot Them Up Game. All Rightes Reserved.

#pragma once

#include "Player/STUPlayerState.h"

class STUUtils
{
public:
	template<typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn)
		{
			return nullptr;
		}

		UActorComponent* Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

	static bool AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2)
		{
			return false;
		}

		ASTUPlayerState* PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
		ASTUPlayerState* PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);
		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}

	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};
