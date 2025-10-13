// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();//确保父类成功初始化
	RunBehaviorTree(BehaviorTree);//启动行为树
	//获取本地玩家的pawn
	APawn* playerpawn = UGameplayStatics::GetPlayerPawn(this,0);
	if (playerpawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", playerpawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", playerpawn);
	}
}