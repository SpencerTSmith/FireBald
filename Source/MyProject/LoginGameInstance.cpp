// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameInstance.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Public/OnlineSubsystemUtils.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineIdentityInterface.h"


void ULoginGameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(this->GetWorld());
	if (Subsystem) 
	{
		IOnlineIdentityPtr IdentityPointer = Subsystem->GetIdentityInterface();
		if (IdentityPointer)
		{
			FOnlineAccountCredentials AccountDetails;

			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;

			IdentityPointer->Login(0, AccountDetails);
			IdentityPointer->OnLoginCompleteDelegates->AddUObject(this, &ULoginGameInstance::LoginWithEOS_Return);
		}
	}
}

FString ULoginGameInstance::GetPlayerUsername()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(this->GetWorld());
	if (Subsystem)
	{
		IOnlineIdentityPtr IdentityPointer = Subsystem->GetIdentityInterface();
		if (IdentityPointer)
		{
			if (IdentityPointer->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return IdentityPointer->GetPlayerNickname(0);
			}
		}
	}
	return FString();
}

bool ULoginGameInstance::IsPlayerLoggedIn()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(this->GetWorld());
	if (Subsystem)
	{
		IOnlineIdentityPtr IdentityPointer = Subsystem->GetIdentityInterface();
		if (IdentityPointer)
		{
			if (IdentityPointer->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return true;
			}
		}
	}
	return false;
}

void ULoginGameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool WasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
	if (WasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Fail Reason - %s"), *Error);
	}
}
