// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIcontroller.h"


/*这个文件的作用是让ai判断当前玩家是否在攻击范围内,增加了该模块之后要编译成功必须在配置文件中增加GamePlayTasks模块，这个函数的作用是每一帧都去判断角色和ai之间的距离*/
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Error, TEXT("✅ 服务节点开始执行")); // 强制显示
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);//确保父类的TickNode先执行

	//检查当前的aipawn和目标actor的距离

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();//获取ai的核心数据储存容器，它允许行为树(BT)、任务(BTTask)和服务(BTService)之间共享和传递数据
	if (ensure(BlackBoardComp))
	{
		//确保这个核心数据容器是存在的

		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));//获取当前AI数据容器中，当前AI的目标Actor是什么
		if (TargetActor)
		{
			UE_LOG(LogTemp, Error, TEXT("检测TargetActor存在"));
			//确保这个目标actor是存在的
			AAIController* Mycontroller = OwnerComp.GetAIOwner();//获取ai的逻辑控制核心
			if(ensure(Mycontroller))
			{
				//确保这个是存在的
				APawn* AIPawn = Mycontroller->GetPawn();//获取
				if (ensure(AIPawn))
				{
					//使用矢量来计算距离

					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());//计算ai和actor之间的欧几里得距离
					bool bwithinRange = (DistanceTo < 2000.0f);//是否存在2000单位阈值以内
					//更新ai数据核心中的值
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bwithinRange);
				}
			}

		}
	}
	UE_LOG(LogTemp, Error, TEXT("✅ 服务节点执行完成"));
}

