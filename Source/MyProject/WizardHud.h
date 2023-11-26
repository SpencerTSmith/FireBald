// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WizardHud.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EHudViewMode : uint8
{
	Minimal UMETA(Tooltip="Little"),
	Moderate UMETA(Tooltip="Normal")
};

inline EHudViewMode& operator++(EHudViewMode& ViewMode)
{
	if (ViewMode == EHudViewMode::Moderate)
		ViewMode = EHudViewMode::Minimal;
	else
		ViewMode = static_cast<EHudViewMode>(static_cast<int>(ViewMode) + 1);

	return ViewMode;
}

inline EHudViewMode& operator--(EHudViewMode& ViewMode)
{
	if (ViewMode == EHudViewMode::Minimal)
		ViewMode = EHudViewMode::Moderate;
	else
		ViewMode = static_cast<EHudViewMode>(static_cast<int>(ViewMode) - 1);

	return ViewMode;
}


UCLASS()
class MYPROJECT_API AWizardHud : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CycleToNextViewMode();

private:
	UPROPERTY(EditAnywhere)
	EHudViewMode CurrentViewMode = EHudViewMode::Moderate;

	void UpdateWidgets();

};
