// Fill out your copyright notice in the Description page of Project Settings.


#include "Paper_Wizard.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectileActor.h"
#include "GameFramework/Character.h"
#include "PaperFlipBookComponent.h"
#include "PaperFlipbook.h"
#include "Components/AudioComponent.h"



// Sets default values
APaper_Wizard::APaper_Wizard()
{
	SetActorTickInterval(0.5f);

	// Camera initialization
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	// Audio for charging sound initialization
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	isMoving = false;
	AttackGate = true;
}



// Called when the game starts or when spawned
void APaper_Wizard::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetRelativeScale3D(FVector(.3f, .3f, .3f));
	GetSprite()->SetUsingAbsoluteRotation(true);
	GetSprite()->SetFlipbook(Flipbooks.IdleRight);
	OnCharacterMovementUpdated.AddDynamic(this, &APaper_Wizard::Animate);
}


void APaper_Wizard::SetCurrentAnimationDirection(FVector const& Velocity)
{
	const float x = GetVelocity().GetSafeNormal().X;
	const float y = GetVelocity().GetSafeNormal().Y;

	isMoving = x != 0.0f || y != 0.0f;

	if (isMoving)
	{
		if (x > 0.0f)
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
	// Don't animate other stuff if attacking
	if (!AttackGate)
		return;

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
		Destroy();
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
	if (CurrentStamina >= SwordStaminaCost && AttackGate)
	{
		// Actual attack collision
		if (SwordClass)
		{
			FVector PlayerLocation = GetActorLocation();
			FRotator Rotation;
			if (CurrentAnimationDirection == EAnimationDirection::Right)
				Rotation = FRotator(0, 360, 0);
			else
				Rotation = FRotator(0, 180, 0);

			// Get a location with offset to spawn at
			FVector SpawnLocation = PlayerLocation + (Rotation.Vector() * SwordOffset);

			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AProjectileActor* Sword = World->SpawnActor<AProjectileActor>(SwordClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
				if (Sword)
				{
					Sword->FireInDirection(Rotation.Vector());
				}
			}
		}

		// Animate it
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.AttackLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.AttackRight);
			break;
		}

		CurrentStamina -= SwordStaminaCost;
		
		AttackGate = false;
		GetWorldTimerManager().SetTimer(AttackTimer,
			[this]()->void
			{
				AttackGate = true;
				GetSprite()->SetRelativeLocation(FVector::ZeroVector);
			}, 0.2f, false);
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

		// animate it
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.StunLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.StunRight);
			break;
		}

		CurrentStamina -= SwordStaminaCost;

		AttackGate = false;
		GetWorldTimerManager().SetTimer(AttackTimer,
			[this]()->void
			{
				AttackGate = true;
				GetSprite()->SetRelativeLocation(FVector::ZeroVector);
			}, 0.2f, false);

		CurrentStamina -= StunStaminaCost;
	}

}

void APaper_Wizard::SpellFireball()
{
	// Can't move while charging
	if (CurrentFireballCharge < MaxFireballCharge && !isMoving)
	{
		const float OldFireballCharge = CurrentFireballCharge;

		AudioComponent->Play();

		// animate it
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.ChargeLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.ChargeRight);
			break;
		}

		AttackGate = false;
		GetWorldTimerManager().SetTimer(AttackTimer,
			[this]()->void
			{
				AttackGate = true;
				GetSprite()->SetRelativeLocation(FVector::ZeroVector);
			}, 0.2f, false);

		CurrentFireballCharge = FMath::Clamp(CurrentFireballCharge + DeltaFireballCharge, 0, MaxFireballCharge);
		OnStaminaChanged.Broadcast(OldFireballCharge, CurrentFireballCharge, MaxFireballCharge);
	}
	else if (CurrentStamina >= FireballStaminaCost && CurrentFireballCharge == MaxFireballCharge)
	{
		FireProjectile(FireballClass, ProjectileOffset);

		// animate it
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.FireLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.FireRight);
			break;
		}

		CurrentStamina -= SwordStaminaCost;

		AttackGate = false;
		GetWorldTimerManager().SetTimer(AttackTimer,
			[this]()->void
			{
				AttackGate = true;
				GetSprite()->SetRelativeLocation(FVector::ZeroVector);
			}, 0.2f, false);

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