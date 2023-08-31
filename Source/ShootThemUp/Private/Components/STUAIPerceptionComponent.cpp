// Shoot Them Up Game. All Rightes Reserved.


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* USTUAIPerceptionComponent::GetClosetEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() == 0) return nullptr;
    }

    AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    APawn* Pawn = Controller->GetPawn();
    if (!Pawn)  return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (AActor* PercieveActor : PercieveActors)
    {
        USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);

        APawn* PercievePawn = Cast<APawn>(PercieveActor);
        const bool bAreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);

        if (HealthComponent && !HealthComponent->IsDead() && bAreEnemies)
        {
            const float CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}
