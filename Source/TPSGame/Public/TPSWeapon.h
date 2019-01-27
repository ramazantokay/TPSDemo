// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSCharacter.h"
#include "TPSWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class TPSGAME_API ATPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSWeapon();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATPSCharacter* MyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FX")
	UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FX")
	UParticleSystem* TraceFX;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* FireSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category="")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float RotationValue;

	void PlayEffects(FVector TraceEnd);

	FTimerHandle TimerHandle_AutoFire;

	float LastFireTime;

	/*RPM - Bullets per minute*/
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float RateofFire;

	/*Derived from RateofFire*/
	float TimeBetweenShots;

public:	

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();
	
	void AutoFireStart();

	void AutoFireStop();
	
	void StartEquipAnimation();

};
