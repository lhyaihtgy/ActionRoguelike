// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;//声明这两个类


UCLASS()
class ACTIONROGUELIKE_API ALCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALCharacter();

protected:
	UPROPERTY(EditAnywhere)//确保下面这个组件在编辑器中可见其属性
	TSubclassOf<AActor> ProjectilesClass;//因为魔法飞弹的父类也是AActor所以这里的这个Pro是可以接受魔法飞弹类的
protected:

	UPROPERTY(visibleAnywhere)//这是声明在UE编辑器中让这个组件的属性能够被蓝图等组件可见可见
	USpringArmComponent* SpringArmComp;//这是一个弹簧臂的指针

	UPROPERTY(visibleAnywhere)
	UCameraComponent* CameraComp;//这是一个摄像机类的指针

	//声明处理前进/后退的函数
	void MoveForward(float value);

	//声明处理左右的函数
	void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//处理释放魔法飞弹的函数
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
