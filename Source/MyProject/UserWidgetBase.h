// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
