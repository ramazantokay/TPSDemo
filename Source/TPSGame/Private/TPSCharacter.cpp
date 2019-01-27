// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "TPSWeapon.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;
	WeaponAttachSocketName = "WeaponSocket";
	WeaponSwapSocket = "WeaponHolster";
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();


	DefaultFOV = CameraComp->FieldOfView;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	CurrentWeapon = GetWorld()->SpawnActor<ATPSWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	SecondWeapon = GetWorld()->SpawnActor<ATPSWeapon>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		//CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		
	}

	if (SecondWeapon)
	{
		SecondWeapon->SetOwner(this);
		SecondWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSwapSocket);
	}

}

void ATPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ATPSCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ATPSCharacter::BeginCrouch()
{
	bIsCrouch = true;
}

void ATPSCharacter::EndCrouch()
{
	bIsCrouch = false;

}

void ATPSCharacter::BeginJump()
{
	bIsJump = true;
}

void ATPSCharacter::EndJump()
{
	bIsJump = false;
}

void ATPSCharacter::StartFire()
{
	bIsFire = true;
	if (CurrentWeapon && bChange)
	{
		CurrentWeapon->AutoFireStart();

	}
	else if (SecondWeapon && !bChange)
	{
		SecondWeapon->AutoFireStart();
	}

}

void ATPSCharacter::StopFire()
{
	bIsFire = false;

	if (CurrentWeapon && bChange)
	{
		CurrentWeapon->AutoFireStop();
	}
	else if (SecondWeapon && !bChange)
	{
		SecondWeapon->AutoFireStop();
	}
}

void ATPSCharacter::SprintStart()
{
	bIsRun = true;
}

void ATPSCharacter::SprintEnd()
{
	bIsRun = false;
}

void ATPSCharacter::ZoomBegin()
{
	bIsZoom = true;
}

void ATPSCharacter::ZoomEnd()
{
	bIsZoom = false;
}


void ATPSCharacter::ChangeWeapon()
{
	if (bChange)
	{

		SecondWeapon->SetOwner(this);
		SecondWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSwapSocket);
		bChange = false;
	}
	else
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		SecondWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSwapSocket);
		bChange = true;
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bIsZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);


}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATPSCharacter::EndJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacter::StopFire);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATPSCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATPSCharacter::SprintEnd);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ATPSCharacter::ZoomBegin);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ATPSCharacter::ZoomEnd);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ATPSCharacter::ChangeWeapon);


}

FVector ATPSCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

