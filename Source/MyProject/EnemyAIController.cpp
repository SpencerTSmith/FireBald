// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyCharacter.h"
#include <Kismet/GameplayStatics.h>

AEnemyAIController::AEnemyAIController()
{
	// Perception Component initialized
	DetectingComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SetPerceptionComponent(*DetectingComponent);

	// Attach sight config to the perception component
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DetectingComponent->ConfigureSense(*SightConfig);

	// If we want to have it based on sight, probably not for this game though
	DetectingComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerDetected);
}

void AEnemyAIController::OnPlayerDetected(const TArray<AActor*>& DetectedActors)
{
	for (AActor* DetectedActor : DetectedActors) 
	{
		if (DetectedActor->IsA(AMyCharacter::StaticClass()))
		{
			AMyCharacter* Player = Cast<AMyCharacter>(DetectedActor);
			float AcceptanceRadius = 1.0f;
				
			if (Player)
				MoveToLocation(Player->GetActorLocation(), AcceptanceRadius);
		}
	}
}

