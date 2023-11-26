// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ModerateLayoutBase1.h"
#include "Paper_Wizard.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UModerateLayoutBase1> ModerateLayoutClass = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetCurrentViewMode(EHudViewMode NewViewMode);

	UFUNCTION(BlueprintCallable)
	void CycleToNextViewMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere)
	EHudViewMode CurrentViewMode = EHudViewMode::Moderate;

	void UpdateWidgets();

	void ClearAllHandlers();
	
	UPROPERTY()
	UWorld* World = nullptr;

	UPROPERTY()
	UModerateLayoutBase1* ModerateLayoutWidget = nullptr;

	UPROPERTY()
	APaper_Wizard* Player = nullptr;

};
