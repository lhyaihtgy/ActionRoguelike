// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void ASDashProjectile::Explode_Implementation()
{
	//首先清楚可能存在的计时器，避免重复触发
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	//在当前位置播放爆炸特效
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	Effectcomp->DeactivateSystem();//停止尾迹特效

	//立即让投射物停止并且禁止碰撞
	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	//设置传送定时器
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);//到达时间后执行传送逻辑

}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();//获取施法者
	if (ensure(ActorToTeleport))
	{
		//保持施法者原有旋转，只改变位置
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);//将 Actor 瞬间移动到指定位置??
	}
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();//执行基类初始化

	//设置延时引爆计时器
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);//当设定时间到达后执行引爆逻辑
}


ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f; //投射物的初始速度
}
