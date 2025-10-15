// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
/*这个文件是实现AI的远程攻击逻辑*/

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();//获取ai操作器
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());//获取ai的移动控制组件
		if (MyPawn == nullptr)
		{
			//Mypawn必须存在否则这个ai是无法发出攻击的
			return EBTNodeResult::Failed;
		}
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");//得到当前ai模型的骨骼网格体，从骨骼插槽中获取射击点位置

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));//获取当前AI数据容器中，当前AI的目标Actor是什么
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		//计算射击方向
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();//转化为旋转，让骨骼转向目标

		FActorSpawnParameters Parms;
		Parms.Instigator = MyPawn;// 将飞弹的发射者设置为AI角色自身
		Parms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//确保即使碰撞也会生成投射物

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(projectileClass, MuzzleLocation, MuzzleRotation, Parms);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
