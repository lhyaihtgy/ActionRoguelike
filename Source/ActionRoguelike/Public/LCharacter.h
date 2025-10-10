// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SInteractionComponent.h"
#include "LCharacter.generated.h" 
/* Unreal Engine C++ 编程中，#include "文件名.generated.h"这条语句​​必须​​放在其头文件（.h文件）中所有其他 #include语句的​​最后面​​。*/
class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API ALCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ALCharacter();

protected:
	
	UPROPERTY(EditAnywhere,Category = "Attack")//确保下面这个组件在编辑器中可见其属性
	TSubclassOf<AActor> ProjectilesClass;//因为魔法飞弹的父类也是AActor所以这里的这个Pro是可以接受魔法飞弹类的

	UPROPERTY(EditAnywhere, Category = "Attack")//确保下面这个组件在编辑器中可见其属性
	TSubclassOf<AActor> DashProjectileClass;//传送的父类也是AActor所i这里也是可以接受黑洞的

	UPROPERTY(EditAnywhere, Category = "Attack")//确保下面这个组件在编辑器中可见其属性
	TSubclassOf<AActor> BlackHoleProjectileClass;//黑洞的父类

	// 动画延迟时间（可编辑，有默认值）
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackAniDelay = 0.2f;


	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAni;//攻击动画

	FTimerHandle FTimerHandle_PrimaryAttack;//魔法飞弹计时器
	FTimerHandle TimerHandle_Dash;//传送计时器
	FTimerHandle TimeHandle_BlackHole;//黑洞
protected:

	UPROPERTY(VisibleAnywhere)//这是声明在UE编辑器中让这个组件的属性能够被蓝图等组件可见可见
	USpringArmComponent* SpringArmComp;//这是一个弹簧臂的指针

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;//这是一个摄像机类的指针

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;//将交互的功能交给人物



	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Componts")
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);//当角色的生命值发生变化的时候需要进行检查

	void SpawnProjectile(TSubclassOf<AActor> ClassTospawn);

	void Dash();
	void Dash_TimeElapsed();//传送技能

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();////让飞弹生成延后让飞弹和攻击动画匹配的函数

	void BlackHoleAttack();
	void BlackholeAttack_TimeElapsed();//黑洞技能

	virtual void PostInitializeComponents() override;//建立角色生命值变化的监听机制​​，确保当角色的生命值发生变化时，能够自动触发相应的处理逻辑

	//声明处理前进/后退的函数
	void MoveForward(float value);

	//声明处理左右的函数
	void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//交互宝箱的函数
	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
