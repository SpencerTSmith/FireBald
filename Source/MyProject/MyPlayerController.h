// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <Components/InputComponent.h>
#include <EnhancedInputLibrary.h>
#include "MyPlayerController.generated.h"


class AMyCharacter;
/**
 * 
 */
UCLASS(Abstract)
class MYPROJECT_API AMyPlayerController : public APlayerController
{
public:

	//Input action to movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Move = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Look = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Jump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Stun = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Sword = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Blink = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* Fireball = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RotationConfiguration", meta = (AllowPrivateAccess = "true"))
	float RotationInterpSpeed{ 120.f };

	//virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleRotation(float DeltaSeconds);
	void HandleJump();
	void HandleSword();
	void HandleStun();
	void HandleFireball();
	void HandleBlink();

private:

	//Store a ref to InputComponent cast to EnhancedInputComponent
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	//Store a ref to pawn we are controlling
	AMyCharacter* PlayerCharacter = nullptr;

	GENERATED_BODY()
};
