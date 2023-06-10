// Shoot Them Up Game. All Rightes Reserved.

#pragma once

class STUUtils
{
public:
	template<typename T>
	static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	{
		if (!PlayerPawn)
		{
			return nullptr;
		}

		UActorComponent* Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
