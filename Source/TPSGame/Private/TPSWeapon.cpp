// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSWeapon.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "TPSGame.h"

// Sets default values
ATPSWeapon::ATPSWeapon()
{

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	RotationValue = 1;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	RateofFire = 600.0f;

}

void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateofFire;
}


void ATPSWeapon::Fire()
{


	//APlayerController* PC = Instigator ? Cast<APlayerController>(Instigator->Controller) : nullptr;
	//FVector FinalAim = FVector::ZeroVector;

	//if (PC)
	//{
	//	FVector CamLoc;
	//	FRotator CamRot;
	//	PC->GetPlayerViewPoint(CamLoc, CamRot);
	//	FinalAim = CamRot.Vector();
	//	DrawDebugLine(GetWorld(), CamLoc, FinalAim, FColor::Green, false, 5.0f, 0, 1.0f);


	//}
	//else if (Instigator)
	//{
	//	FinalAim = Instigator->GetBaseAimRotation().Vector();

	//}

	//APlayerController* PC = Cast<APlayerController>(Instigator->GetController());

	//
	//FVector EyeLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

	//FRotator EyeRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
	//PC->GetPlayerViewPoint(EyeLocation, EyeRotation);
	//GetActorEyesViewPoint(EyeLocation, EyeRotation);


	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{

		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
		FVector TraceEndPoint = TraceEnd;

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 5.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			float ActualDamage = BaseDamage;
		}




		PlayEffects(TraceEndPoint);



		LastFireTime = GetWorld()->TimeSeconds;

	}

}

void ATPSWeapon::AutoFireStart()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ATPSWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ATPSWeapon::AutoFireStop()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
}

void ATPSWeapon::StartEquipAnimation()
{
	MyPawn->PlayAnimMontage(EquipAnim, 1.f,NAME_None);
}

void ATPSWeapon::PlayEffects(FVector TraceEnd)
{
	if (MuzzleFX)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFX, MeshComp, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (TraceFX)
	{

		FVector Origin = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShootDir = TraceEnd - Origin;
		ShootDir.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(this, TraceFX, Origin, ShootDir.Rotation());


		//FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		//UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceFX, MuzzleLocation);
		//if (TracerComp)
		//{
		//	TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		//}
	}




	//Camera Shake
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}




}
