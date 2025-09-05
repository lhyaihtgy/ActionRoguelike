// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent/*这个类专门用来管理各种属性buff，不如血量*/
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	/*
	EditAnywhere​​ - 可在蓝图编辑器中和关卡内的每个实例上进行编辑。
​​	VisibleAnywhere​​ - 在编辑器中和关卡内为“只读”（常用于组件）。
​​	EditDefaultsOnly​​ - 对每个实例隐藏变量，仅在蓝图编辑器中可编辑。
​​	VisibleDefaultsOnly​​ - 变量具有“只读”权限，且仅在蓝图编辑器中可见（不常用）。
​​	EditInstanceOnly​​ - 仅允许编辑实例（例如，当放置在关卡中时）。
​​	BlueprintReadOnly​​ - 在蓝图脚本中为只读（不影响“详细信息”面板中的编辑权限）。
​​	BlueprintReadWrite​​ - 在蓝图脚本中具有读写权限。
​​	Category = ""​​ - 该分类仅用于详情面板和蓝图上下文菜单的显示组织。类似于文件夹，在蓝图的细节面板中category的名字是一个属性，而所有该属性的变量/对象都会在该属性面板下*/

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float health;//


public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

};
