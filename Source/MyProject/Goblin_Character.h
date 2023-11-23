// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "Paper_Wizard.h"
#include "Goblin_Character.generated.h"

UCLASS()
class MYPROJECT_API AGoblin_Character : public APaperCharacter
{
	GENERATED_BODY()

private:


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goblin|Animation")
	EAnimationDirection CurrentAnimationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goblin|Animation")
	FAnimationFlipbooks Flipbooks;

	UFUNCTION(BlueprintCallable, Category = "Goblin|Animation")
	void SetCurrentAnimationDirection(FVector const& Velocity);

	UFUNCTION(BlueprintCallable, Category = "Goblin|Animation")
	void Animate(float DeltaTime, FVector OldLocation, FVector NewLocation);
public:
	AGoblin_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Config")
	bool isMoving;
};
