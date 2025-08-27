// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{

	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;//最基本的球形碰撞组件，处理碰撞和交互

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;//抛射物移动组件，提供自动的基于物理的移动能力

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp;//视觉特效组件

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
