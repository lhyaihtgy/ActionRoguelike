// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include <SGameplayInterface.cpp>

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParms;
	ObjectQueryParms.AddObjectTypesToQuery(ECC_WorldDynamic);/*以上两行代码，创建一个 FCollisionObjectQueryParams对象来配置射线检测要检测哪些类型的物体。AddObjectTypesToQuery(ECC_WorldDynamic)
	表示这条射线​​只检测​​标记为 WorldDynamic类型的物体。这类物体通常是场景中可移动、可交互的物体（如宝箱、门、道具），而不会检测到墙壁、地板等静态环境（WorldStatic）。这是一种性能优化和逻辑过滤。*/

	AActor* MyOwner = GetOwner();//USInteractionComponent组件所依附的 Actor。在大多数情况下，这个 MyOwner就是​​玩家控制的角色（Pawn）​​。

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//它会根据 Actor 的类型返回一个合理的“视角”位置。对于角色，它通常返回相机的位置；对于其他 Actor，可能返回其自身的位置。将返回的值放入到两个变量中

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);//计算射线的终点，EyeRotation.Vector()将旋转器转换成一个表示方向的向量。

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParms);//GetWorld()->LineTraceSingleByObjectType：在游戏世界中执行一条单条射线检测。
	////bool反映了是否射中了某些物体

	//AActor* HitActor = Hit.GetActor();//获取该次射线是否射中了AActor物品
	//if (HitActor)
	//{
	//	if (HitActor->Implements<USGameplayInterface>())////检测这个射中的物体是否实现了交互接口
	//	{
	//		APawn* MyPawn = Cast<APawn>(MyOwner); //将组件的所有者(MyOwner) 尝试转换为 APawn类型。这是因为交互接口的 Interact函数通常需要一个 APawn* （即“谁发起了交互”）作为参数。
	//		ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
	//	}
	//}//以上是第一版本代码，这个版本的检测需要玩家比较精准的瞄准到宝箱之后才能触发互动
	/*第二个版本，依旧是10厘米只不过从单条射线检测（Line Trace）改为球形范围扫描（Sphere Sweep）​​。这会使交互系统的体验从“​​用绣花针精确点击​​”变为“​​用手掌范围性地摸索​​”。*/

	TArray<FHitResult> Hits;//储存在球形范围扫描检测的多个结果

	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);//设定该次球形扫描球形的大小为30cm
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParms, Shape);
	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;//当这个检测到物体的时候会发出绿色的光，反之则是红色
	/*FQuat::Identity：表示扫描体在此过程中​​不进行旋转​​（使用单位四元数，即无旋转）。
	ObjectQueryParams：碰撞对象查询参数（之前设置的只检测ECC_WorldDynamic物体）。shape就是扫描所用的文件*/

	for (FHitResult Hit:Hits)//遍历所有扫描到的物品
	{
		AActor* HitActor = Hit.GetActor();//获取该次扫描是否射中了AActor物品
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())////检测这个扫描的物体是否实现了交互接口
			{
				APawn* MyPawn = Cast<APawn>(MyOwner); //将组件的所有者(MyOwner) 尝试转换为 APawn类型。这是因为交互接口的 Interact函数通常需要一个 APawn* （即“谁发起了交互”）作为参数。
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, DebugColor, false, 2.0f);
				break;//触发一次之后就退出循环，防止出现很近的宝箱或者其他被一起打开的情况
			}
		}
		/* Hit.ImpactPoint是球体的中心点​​ 32是球体的细分精度​​。这个数字决定了球体由多少条线段拟合而成。数值越高，球体看起来越圆滑，但计算开销也略大。32 是一个在视觉效果和性能之间取得良好平衡的常用值。
		布尔值​​为是否启用深度检测​​。如果为 false，这个调试球体将始终绘制在最上层，不会被场景中的其他物体遮挡，确保您总能看见它。*/
	}


	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.0F, 0, 2.0f);
}
//从玩家视角发射一条射线（Raycast），检测玩家正在看什么物体。如果该物体可以被交互，就触发它的交互行为。​
