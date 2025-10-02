// Fill out your copyright notice in the Description page of Project Settings.
/*这个类是作为新技能魔法黑洞的基类使用的*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT)/*ABSTRACT​​：这是关键设计！标记此类为​​抽象类​​，意味着：不会在编辑器的"生成Actor"下拉菜单中显示不能直接创建实例，必须创建子类来使用鼓励代码复用和标准化*/
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX; // 碰撞特效资源

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;  //球形碰撞组件，定义投射物的​​物理碰撞边界​

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp; //投射物的移动组件，提供​​物理模拟的运动控制​

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* Effectcomp; //粒子系统组件，附着在投射物上的持续视觉效果​

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();//这个函数使用了BlueprintNativeEvent标签代表这个函数在蓝图进行实现

	virtual void PostInitializeComponents() override;

};
