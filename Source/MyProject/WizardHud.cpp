// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHud.h"

void AWizardHud::SetCurrentViewMode(EHudViewMode NewViewMode)
{
}

void AWizardHud::CycleToNextViewMode()
{
	++CurrentViewMode;
	UE_LOG(LogTemp, Warning, TEXT("CycleToNextViewMode %s"), *UEnum::GetValueAsString(CurrentViewMode));
	UpdateWidgets();
}

void AWizardHud::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	ModerateLayoutWidget = CreateWidget<UModerateLayoutBase1>(World, ModerateLayoutClass);
	ModerateLayoutWidget->AddToViewport();
	ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);
	

	if (APlayerController* PlayerController = GetOwningPlayerController())
		Player = Cast<APaper_Wizard>(PlayerController->GetPawn());

	UpdateWidgets();
}

void AWizardHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void AWizardHud::UpdateWidgets()
{
	Player->OnHealthChanged.AddDynamic(ModerateLayoutWidget->StatBarGroup->HealthBar, &UStatBarBase::OnIntStatUpdated);
	Player->OnStaminaChanged.AddDynamic(ModerateLayoutWidget->StatBarGroup->StaminaBar, &UStatBarBase::OnFloatStatUpdated);
	Player->OnFireballChargeChanged.AddDynamic(ModerateLayoutWidget->StatBarGroup->ChargeBar, &UStatBarBase::OnFloatStatUpdated);
	ModerateLayoutWidget->SetVisibility(ESlateVisibility::Visible);

	Player->BroadcastCurrentStats();
}

void AWizardHud::ClearAllHandlers()
{
	if (Player)
	{
		Player->OnHealthChanged.Clear();
		Player->OnStaminaChanged.Clear();
		Player->OnFireballChargeChanged.Clear();
	}
}
