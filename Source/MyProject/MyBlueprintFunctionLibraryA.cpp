// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibraryA.h"

FString UMyBlueprintFunctionLibraryA::GetMessage()
{
	return FString(TEXT("Message"));
}

double UMyBlueprintFunctionLibraryA::GetMyPi()
{
	return Pi;
}

FAverages UMyBlueprintFunctionLibraryA::CalculateAverages(const TArray<int32>& InValues)
{
	return FAverages();
}
