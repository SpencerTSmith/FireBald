// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	this->HandleRotation(DeltaSeconds);
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Store a ref to player's pawn
	PlayerCharacter = Cast<AMyCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("AMyPlayerController derived classes should only possess AMyCharacter derived pawns"));

	// Get ref to EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Can't reference EnhancedInputComponent"));

	// Get local player subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Can't get ref to EnhancedInputLocalPlayerSubsystem"));

	// Wipe existing mappings, and add our mapping.
	checkf(InputMappingContext, TEXT("InputMappingContext was not specified."));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	// Bind actions
	if (Move)
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleMove);
	if (Look)
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleLook);
	if (Jump)
		EnhancedInputComponent->BindAction(Jump, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleJump);
	if (Sword)
		EnhancedInputComponent->BindAction(Sword, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleSword);
	if (Stun)
		EnhancedInputComponent->BindAction(Stun, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleStun);
	if (Fireball)
		EnhancedInputComponent->BindAction(Fireball, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleFireball);
	if (Blink)
		EnhancedInputComponent->BindAction(Blink, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleBlink);

	// Show cursor
	this->bShowMouseCursor = true;
}

void AMyPlayerController::OnUnPossess()
{
	EnhancedInputComponent->ClearActionBindings();
	Super::OnUnPossess();
}



void AMyPlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	/*const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);*/
}

void AMyPlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2d>();
	if (PlayerCharacter)
	{
		// Relative to player
		/*PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);*/
		
		// Relative to camera
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetCameraComponent()->GetForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetCameraComponent()->GetRightVector(), MovementVector.X);
	}
}

void AMyPlayerController::HandleRotation(float DeltaSeconds)
{
	FVector MousePosition, MouseDirection;

	if (this->DeprojectMousePositionToWorld(MousePosition, MouseDirection))
	{
		// My Implementation that doesn't work
		/*FRotator CurrentCharacterRotation = PlayerCharacter->GetActorRotation();
		FRotator TargetRotation = MouseDirection.Rotation();

		FRotator NewRotation = FRotator(CurrentCharacterRotation.Pitch, TargetRotation.Yaw, CurrentCharacterRotation.Roll);

		PlayerCharacter->SetActorRotation(NewRotation);*/
		
		// This implementation works
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		
		FVector MouseDirectionAdjusted = PlayerLocation + (MouseDirection * 1000.f);
		MouseDirectionAdjusted.Z = PlayerLocation.Z;
		FVector EndLocation = FMath::LinePlaneIntersection(
			MousePosition,
			MousePosition + (MouseDirection * 10000.f),
			PlayerLocation,
			FVector{ 0.f, 0.f, 1.f }
		);
		// Same z plane
		EndLocation.Z = PlayerLocation.Z;

		// Rotation between current player location and mouse location
		FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EndLocation);
		// Don't rotate these directions
		DesiredRotation.Roll = 0.f;
		DesiredRotation.Pitch = 0.f;
		// Interpolate to smooth
		FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), DesiredRotation, DeltaSeconds, RotationInterpSpeed);

		SetControlRotation(NewRotation);
	}
}

void AMyPlayerController::HandleJump()
{
	if (PlayerCharacter)
	{
		//PlayerCharacter->Jump();
	}
}

void AMyPlayerController::HandleSword()
{
	if (PlayerCharacter) {
		PlayerCharacter->AttackSword();
	}
}

void AMyPlayerController::HandleStun()
{
	if (PlayerCharacter) {
		PlayerCharacter->SpellStun();
	}
}

void AMyPlayerController::HandleFireball()
{
	if (PlayerCharacter) {
		PlayerCharacter->SpellFireball();
	}
}

void AMyPlayerController::HandleBlink()
{
	if (PlayerCharacter) {
		PlayerCharacter->SpellBlink ();
	}
}