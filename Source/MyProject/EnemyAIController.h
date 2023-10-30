// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AISenseConfig_Sight.h>
#include "EnemyAIController.generated.h"



/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AEnemyAIController();

protected:

	UPROPERTY(EditAnywhere, Category = "Perception")
	UAIPerceptionComponent* DetectingComponent;
	UPROPERTY(EditAnywhere, Category = "Perception")
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION(BlueprintCallable, Category = "Perception")
	void OnPlayerDetected(const TArray<AActor*>& DetectedActors);

private:

};
