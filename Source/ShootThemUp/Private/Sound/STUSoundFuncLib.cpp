// Shoot Them Up Game. All Rightes Reserved.


#include "Sound/STUSoundFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSoundFuncLib, All, All);

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (!SoundClass)
	{
		return;
	}

	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
	UE_LOG(LogSTUSoundFuncLib, Display, TEXT("Sound class volume was changed : %s = %f"), *SoundClass->GetName(), SoundClass->Properties.Volume);
}

void USTUSoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (!SoundClass)
	{
		return;
	}

	const float NextVolume = SoundClass->Properties.Volume > 0.f ? 0.f : 1.f;
	SetSoundClassVolume(SoundClass, NextVolume);
}
