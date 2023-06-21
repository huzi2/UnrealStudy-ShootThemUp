// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void USTUBaseWidget::Show()
{
	PlayAnimation(ShowAnimation);

	if (GetWorld() && OpenSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
	}
}
