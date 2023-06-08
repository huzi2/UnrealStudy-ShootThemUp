// Shoot Them Up Game. All Rightes Reserved.


#include "Animation/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	OnNotified.Broadcast(MeshComp);
}
