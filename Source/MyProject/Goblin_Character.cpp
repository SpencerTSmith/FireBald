// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin_Character.h"
#include "PaperFlipbookComponent.h"

AGoblin_Character::AGoblin_Character()
{
	isMoving = true;
}

void AGoblin_Character::BeginPlay()
{
	Super::BeginPlay();

	OnCharacterMovementUpdated.AddDynamic(this, &AGoblin_Character::Animate);
}

void AGoblin_Character::SetCurrentAnimationDirection(FVector const& Velocity)
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

void AGoblin_Character::Animate(float DeltaTime, FVector OldLocation, FVector OldVelocity)
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
}


