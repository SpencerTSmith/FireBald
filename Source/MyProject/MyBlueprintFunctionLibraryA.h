// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibraryA.generated.h"

USTRUCT(BlueprintType)
struct FAverages
{
	FAverages() :
		Mean(0.0f),
		Modal(0.0f),
		Median(0.0f) {};

	FAverages(float InMean, float InMode, float InMedian) :
		Mean(InMean),
		Modal(InMode),
		Median(InMedian) {};

	UPROPERTY(BlueprintReadOnly)
	float Mean = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float Modal = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float Median = 0.0f;

	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyBlueprintFunctionLibraryA : public UBlueprintFunctionLibrary
{
	

public:

	UFUNCTION(BlueprintCallable)
	static FString GetMessage();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (ToolTip = "3.14", CompactNodeTitle = "Pi"))
	static double GetMyPi();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (ToolTip = "Calculate mean, mode, and median for an array of ints"))
	static FAverages CalculateAverages(const TArray<int32>& InValues);
protected:

private:
	static constexpr double Pi = 3.14;


	GENERATED_BODY()
};
