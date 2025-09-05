// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChestActor.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChestActor : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()
public:
	float TargetPitch;//用于宝箱打开的角度
	void Interact_Implementation(APawn* instigatorPawn);/*UE在继承了抽象类之后，对于抽象类中的方法必须​​固定的命名约定（_Implementation后缀）​*/
public:	
	// Sets default values for this actor's properties
	ASItemChestActor();

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh; //为这个类增加两个静态网格体这是宝箱的底座

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;//这是宝箱的盖/*VisibleAnywhere：表示该属性在​​所有属性窗口​​（如细节面板、蓝图编辑器）中都可以看到。BlueprintReadOnly：表示该属性在​​蓝图中是可见且可读的​​（但不能被修改）。*/


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
