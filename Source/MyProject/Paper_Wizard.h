// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Paperflipbook.h"
#include "Paper_Wizard.generated.h"

// Delegate for player death
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

// Delegate for int stat change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
	int32, OldValue, int32, NewValue, int32, MaxValue);

// Delegate for float stat change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
	float, OldValue, float, NewValue, float, MaxValue);

UENUM(BlueprintType, Category = "Animation")
enum class EAnimationDirection : uint8
{
	Left,
	Right
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleRight{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunRight{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* AttackRight{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* AttackLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* StunRight{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* StunLeft{ nullptr };
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API APaper_Wizard : public APaperCharacter
{
public:
	// Sets default values for this character's properties
	APaper_Wizard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Magic")
	TSubclassOf<class AProjectileActor> StunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Sword")
	TSubclassOf<class AProjectileActor> SwordClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Magic")
	TSubclassOf<class AProjectileActor> FireballClass;

	// Camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraArm = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera = nullptr;

	// Animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Animation")
	EAnimationDirection CurrentAnimationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animation")
	FAnimationFlipbooks Flipbooks;

	UFUNCTION(BluePrintCallable, Category = "Player|Animation")
	void SetCurrentAnimationDirection(FVector const& Velocity);

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void Animate(float DeltaTime, FVector OldLocation, FVector OldVelocity);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Config")
	bool isMoving;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Player|Health")
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

	UPROPERTY(BlueprintAssignable, Category = "Player|Magic")
	FFloatStatUpdated OnFireballChargeChanged;

	UFUNCTION(BlueprintPure, Category = "Player|Camera")
	UCameraComponent* GetCameraComponent();

	UFUNCTION(BlueprintCallable, Category = "Player|Sword")
	void AttackSword();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Magic")
	int SwordOffset;

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellBlink();

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellStun();

	UFUNCTION(BlueprintCallable, Category = "Player|Magic")
	void SpellFireball();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Magic")
	int ProjectileOffset;

private:

	// Health
	static constexpr int BaseHealth = 30;
	int MaxHealth = BaseHealth;
	int CurrentHealth = BaseHealth;

	// Stamina
	static constexpr float BaseStamina = 20.0f;
	static constexpr float BlinkStaminaCost = 1.0f;
	static constexpr float SwordStaminaCost = 3.0f;
	static constexpr float FireballStaminaCost = 20.0f;
	static constexpr float StunStaminaCost = 10.0f;
	float MaxStamina = BaseStamina;
	float CurrentStamina = BaseStamina;
	float StaminaRestorationFactor = 1.5f;

	// Fireball Charge
	static constexpr float MaxFireballCharge = 50.0f;
	static constexpr float StartingFireballCharge = 0.0f;
	static constexpr float DeltaFireballCharge = 5.0f;
	static constexpr float FireballChargeDecay = 0.05;
	float CurrentFireballCharge = StartingFireballCharge;

	// Projectile Helper
	void FireProjectile(TSubclassOf<class AProjectileActor> ProjectileClass, int Offset);

	FTimerHandle AttackTimer;
	bool AttackGate;

	GENERATED_BODY()
};
