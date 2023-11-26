// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHud.h"

void AWizardHud::CycleToNextViewMode()
{
	++CurrentViewMode;
	UE_LOG(LogTemp, Warning, TEXT("CycleToNextViewMode %s"), *UEnum::GetValueAsString(CurrentViewMode));
	UpdateWidgets();
}

void AWizardHud::UpdateWidgets()
{
}
