// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS(Abstract)
class MYPROJECT_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

private:

	
};
