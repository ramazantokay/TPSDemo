// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSWeapon.h"
#include "TPSGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API ATPSGrenadeLauncher : public ATPSWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;
	
	
};
