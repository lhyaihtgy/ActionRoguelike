// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/Particlesystem.h"
#include "Particles/Particlesystemcomponent.h"
#include "kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/GameFramework/NavMovementComponent.h>

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
	RootComponent = SphereComp;

	Effectcomp = CreateDefaultSubobject<UParticleSystemComponent>("Effectcomp");
	Effectcomp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASProjectileBase::Explode_Implementation()
{
	//if判断确保对象还存在
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

// 这是重写父类的虚函数，不需要在.h中显式声明
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
