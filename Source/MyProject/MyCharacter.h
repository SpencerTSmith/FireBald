// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

// Delegate for player death
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

// Delegate for int stat change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
	int32, OldValue, int32, NewValue, int32, MaxValue);

// Delegate for float stat change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
	float, OldValue, float, NewValue, float, MaxValue);

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category="Player|Health")
	int GetHealth();

	UFUNCTION(BlueprintPure, Category = "Player|Health")
	int GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	void UpdateHealth(int DeltaHealth);

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	void RestoreToMaxHealth();

	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FIntStatUpdated OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FPlayerIsDead OnPlayerDied;

	UFUNCTION(BlueprintPure, Category = "Player|Stamina")
	float GetStamina();

	UFUNCTION(BlueprintPure, Category = "Player|Stamina")
	float GetStaminaRestorationFactor();

	UPROPERTY(BlueprintAssignable, Category = "Player|Stamina")
	FFloatStatUpdated OnStaminaChanged;

	UFUNCTION(BlueprintCallable, Category = "Player|Sword")
	void AttackSword();

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellBlink();

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellStun();

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellFireball();

private:

	// Camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraArm = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera = nullptr;

	// Health
	static constexpr int BaseHealth = 10;
	int MaxHealth = BaseHealth;
	int CurrentHealth = BaseHealth;

	// Stamina
	static constexpr float BaseStamina = 20.0f;
	static constexpr float BlinkStaminaCost = 1.0f;
	static constexpr float SwordStaminaCost = 1.0f;
	static constexpr float FireballStaminaCost = 20.0f;
	static constexpr float StunStaminaCost = 10.0f;
	float MaxStamina = BaseStamina;
	float CurrentStamina = BaseStamina;
	float StaminaRestorationFactor = 0.0f;

	// Fireball Charge
	static constexpr float MaxFireballCharge = 50.0f;
	static constexpr float StartingFireballCharge = 0.0f;
	static constexpr float DeltaFireballCharge = 5.0f;
	float CurrentFireballCharge = StartingFireballCharge;

	GENERATED_BODY()
};
