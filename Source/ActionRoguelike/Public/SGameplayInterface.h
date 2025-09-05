// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent) //BlueprintNativeEvent标签让蓝图也能够重写这个函数,BlueprintCallable将 C++ 中声明的函数暴露给蓝图系统（Blueprint Visual Scripting），使得蓝图可以直接调用这个函数。​
	void Interact(APawn* InstigatorPawn);//APawn致命触发该函数的pawn
};

/*接口是一种契约，它规定了一个类“必须做什么”，但不规定“怎么做”。任何继承了此接口的类，都​​必须​​实现接口中声明的 Interact函数，从而保证所有可交互物体都拥有一个统一的交互方法。*/