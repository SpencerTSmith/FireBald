// Fill out your copyright notice in the Description page of Project Settings.


#include "Paper_Wizard.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectileActor.h"
#include "GameFramework/Character.h"
#include "PaperFlipBookComponent.h"
#include "PaperFlipbook.h"


// Sets default values
APaper_Wizard::APaper_Wizard()
{
	SetActorTickInterval(0.5f);

	// Camera initialization
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	

	isMoving = false;


}



// Called when the game starts or when spawned
void APaper_Wizard::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetFlipbook(RunLeft);
}


void APaper_Wizard::SetCurrentAnimationDirection(const FVector& Velocity)
{
	FVector Forward = GetActorForwardVector().GetSafeNormal();
	FVector Right = GetActorRightVector().GetSafeNormal();

	const float ForwardSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Forward) * 100) / 100;
	const float RightSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Right) * 100) / 100;

	isMoving = ForwardSpeed != 0.0f || RightSpeed != 0.0f;

	if (isMoving)
	{
		if (ForwardSpeed > 0.0f && (RightSpeed < 0.0f || RightSpeed == 0.0f))
		{
			CurrentAnimationDirection = EAnimationDirection::Right;
		}
		else
		{
			CurrentAnimationDirection = EAnimationDirection::Left;
		}
	}
}

void APaper_Wizard::Animate(float DeltaTime, FVector OldLocation, FVector OldVelocity)
{
	SetCurrentAnimationDirection(OldVelocity);

	if (isMoving)
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.RunLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.RunRight);
			break;
		}
	}
	else
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.IdleLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.IdleRight);
			break;
		}
	}
}

// Called every frame
void APaper_Wizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Stamina Regen
	if (CurrentStamina != MaxStamina)
	{
		const float OldStamina = CurrentStamina;

		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRestorationFactor, 0, MaxStamina);
		OnStaminaChanged.Broadcast(OldStamina, CurrentStamina, MaxStamina);
	}

	// Fireball Charge Decay
	if (CurrentFireballCharge != 0)
	{
		const float OldFireballCharge = CurrentFireballCharge;

		CurrentFireballCharge = FMath::Clamp(CurrentFireballCharge - FireballChargeDecay, 0, MaxFireballCharge);
		OnFireballChargeChanged.Broadcast(OldFireballCharge, CurrentFireballCharge, MaxFireballCharge);
	}

	// Update temp UI
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Red, *(FString::Printf(TEXT("Health - Current: %d | Max: %d"), CurrentHealth, MaxHealth)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Green, *(FString::Printf(TEXT("Stamina - Current: %f | Max: %f"), CurrentStamina, MaxStamina)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Orange, *(FString::Printf(TEXT("Charge - Current: %f | Max: %f"), CurrentFireballCharge, MaxFireballCharge)));

}

// Called to bind functionality to input
void APaper_Wizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int APaper_Wizard::GetHealth()
{
	return CurrentHealth;
}

int APaper_Wizard::GetMaxHealth()
{
	return MaxHealth;
}

void APaper_Wizard::UpdateHealth(int DeltaHealth)
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

void APaper_Wizard::RestoreToMaxHealth()
{
	CurrentHealth = MaxHealth;
}

float APaper_Wizard::GetStamina()
{
	return CurrentStamina;
}

float APaper_Wizard::GetStaminaRestorationFactor()
{
	return StaminaRestorationFactor;
}

void APaper_Wizard::AttackSword()
{
	if (CurrentStamina >= SwordStaminaCost)
	{
		FireProjectile(SwordClass, SwordOffset);
		CurrentStamina -= SwordStaminaCost;
	}
}

void APaper_Wizard::SpellBlink()
{
	if (CurrentStamina >= BlinkStaminaCost)
	{
		// to do

		CurrentStamina -= BlinkStaminaCost;
	}
}

void APaper_Wizard::SpellStun()
{
	if (CurrentStamina >= StunStaminaCost)
	{
		FireProjectile(StunClass, ProjectileOffset);
		CurrentStamina -= StunStaminaCost;
	}

}

void APaper_Wizard::SpellFireball()
{
	// Can't move while charging
	if (CurrentFireballCharge < MaxFireballCharge && GetVelocity() == FVector(0))
	{
		const float OldFireballCharge = CurrentFireballCharge;

		CurrentFireballCharge = FMath::Clamp(CurrentFireballCharge + DeltaFireballCharge, 0, MaxFireballCharge);
		OnStaminaChanged.Broadcast(OldFireballCharge, CurrentFireballCharge, MaxFireballCharge);

	}
	else if (CurrentStamina >= FireballStaminaCost && CurrentFireballCharge == MaxFireballCharge)
	{
		FireProjectile(FireballClass, ProjectileOffset);
		CurrentStamina -= FireballStaminaCost;
		CurrentFireballCharge = 0.0f;
	}
}

UCameraComponent* APaper_Wizard::GetCameraComponent()
{
	return Camera;
}

void APaper_Wizard::FireProjectile(TSubclassOf<class AProjectileActor> ProjectileClass, int Offset)
{
	if (ProjectileClass)
	{
		FVector PlayerLocation = GetActorLocation();
		FRotator PlayerRotation = GetActorRotation();

		// Get a location with offset to spawn at
		FVector SpawnLocation = PlayerLocation + (PlayerRotation.Vector() * Offset);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AProjectileActor* Projectile = World->SpawnActor<AProjectileActor>(ProjectileClass, SpawnLocation, PlayerRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->FireInDirection(PlayerRotation.Vector());
			}
		}
	}
}