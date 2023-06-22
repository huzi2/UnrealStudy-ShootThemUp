// Shoot Them Up Game. All Rightes Reserved.


#include "STUGameInstance.h"
#include "Sound/STUSoundFuncLib.h"

USTUGameInstance::USTUGameInstance()
	: MenuLevelName(NAME_None)
{
}

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
