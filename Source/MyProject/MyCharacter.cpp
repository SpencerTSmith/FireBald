// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorTickInterval(0.5f);
	SetActorTickEnabled(true);

	// Camera initialization
	/*CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(GetMesh());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);*/
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Stamina Regen
	if (CurrentStamina != MaxStamina) 
	{
		const float OldStamina = CurrentStamina;

		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRestorationFactor, 0, MaxStamina);
		OnStaminaChanged.Broadcast(OldStamina, CurrentStamina, MaxStamina);
	}

	// Update temp UI
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Red, *(FString::Printf(TEXT("Health - Current: %d | Max: %d"), CurrentHealth, MaxHealth)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Green, *(FString::Printf(TEXT("Stamina - Current: %f | Max: %f"), CurrentStamina, MaxStamina)));
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AMyCharacter::GetHealth()
{
	return CurrentHealth;
}

int AMyCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void AMyCharacter::UpdateHealth(int DeltaHealth)
{
	if (CurrentHealth <= 0.0f)
		return;
	
	int OldHealth = CurrentHealth;

	CurrentHealth += DeltaHealth;

	// Make sure health makes sense
	CurrentHealth = FMath::Clamp(CurrentHealth, -1.f, MaxHealth);

	if (OldHealth != CurrentHealth) 
	{
		OnHealthChanged.Broadcast(OldHealth, CurrentHealth, MaxHealth);
	}

	if (CurrentHealth <= 0.0f) 
	{
		// Death screen
	}
}

void AMyCharacter::RestoreToMaxHealth()
{
	CurrentHealth = MaxHealth;
}

float AMyCharacter::GetStamina()
{
	return CurrentStamina;
}

float AMyCharacter::GetStaminaRestorationFactor()
{
	return StaminaRestorationFactor;
}

void AMyCharacter::AttackSword()
{
	if (CurrentStamina >= SwordStaminaCost)
	{
		// to do

		CurrentStamina -= SwordStaminaCost;
	}
}

void AMyCharacter::SpellBlink()
{
	if (CurrentStamina >= BlinkStaminaCost)
	{
		// to do

		CurrentStamina -= BlinkStaminaCost;
	}
}

void AMyCharacter::SpellStun()
{
	if (CurrentStamina >= StunStaminaCost)
	{
		// to do

		CurrentStamina -= StunStaminaCost;
	}
	
}

void AMyCharacter::SpellFireball()
{
	if (CurrentStamina >= FireballStaminaCost && CurrentFireballCharge == MaxFireballCharge)
	{
		// to do

		CurrentStamina -= FireballStaminaCost;
		CurrentFireballCharge = 0.0f;
	}
}

