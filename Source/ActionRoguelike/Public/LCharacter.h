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

	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAni;

	FTimerHandle FTimerHandle_PrimaryAttack;//计时器
protected:

	UPROPERTY(VisibleAnywhere)//这是声明在UE编辑器中让这个组件的属性能够被蓝图等组件可见可见
	USpringArmComponent* SpringArmComp;//这是一个弹簧臂的指针

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;//这是一个摄像机类的指针

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;//将交互的功能交给人物

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Componts")
	USAttributeComponent* AttributeComp;

	//声明处理前进/后退的函数
	void MoveForward(float value);

	//声明处理左右的函数
	void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//处理释放魔法飞弹的函数
	void PrimaryAttack();

	//交互宝箱的函数
	void PrimaryInteract();
	//让飞弹生成延后让飞弹和攻击动画匹配的函数
	void PrimaryAttack_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
