// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,Category = "Teleport")
	float TeleportDelay; // 传送延迟的时间

	UPROPERTY(EditDefaultsOnly,Category = "Teleport")
	float DetonateDelay; //引爆延迟时间

	FTimerHandle TimerHandle_DelayedDetonate; // 确保在指定时间后执行特定操作的定时器

	virtual void Explode_Implementation() override; //重写爆炸函数

	void TeleportInstigator();

	virtual void BeginPlay() override;
public:
	ASDashProjectile();
};
