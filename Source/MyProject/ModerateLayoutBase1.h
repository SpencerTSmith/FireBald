// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "StatBarGroup.h"
#include "ModerateLayoutBase1.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UModerateLayoutBase1 : public UUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Constituent Controls", meta = (BindWidget))
	TObjectPtr<UStatBarGroup> StatBarGroup = nullptr;
	
};
