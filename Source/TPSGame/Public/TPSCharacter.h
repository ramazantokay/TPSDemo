// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATPSWeapon;

UCLASS()
class TPSGAME_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArmComp;

	ATPSWeapon* CurrentWeapon;

	ATPSWeapon* SecondWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ATPSWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ATPSWeapon> SecondaryWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Player")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category="Player")
	FName WeaponSwapSocket;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomedFOV;

	float DefaultFOV;


	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void BeginJump();

	void EndJump();

	void StartFire();

	void StopFire();

	void SprintStart();
	
	void SprintEnd();

	void ZoomBegin();

	void ZoomEnd();

	void ChangeWeapon();

	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bIsCrouch;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bIsJump;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bIsFire;

	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bIsRun;
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bIsZoom;

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	bool bChange = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	
	
	
};
