// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LoginGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API ULoginGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions")
	bool IsPlayerLoggedIn();

	void LoginWithEOS_Return(int32 LocalUserNum, bool WasSucces, const FUniqueNetId& UserId, const FString& Error);
};
