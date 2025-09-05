// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();//：这是一个​​标准且重要​​的操作。它首先调用父类（例如 AActor）的 PostInitializeComponents 函数，确保引擎内部的基础初始化逻辑得以执行。忽略这一步可能会导致不可预知的错误。
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActionHit);////绑定事件委托​​，这行代码建立了“​​当桶被击中时，就调用 OnActorHit 函数来处理这次击中​​”的响应机制。
	/*在这里进行绑定是因为:​​ PostInitializeComponents 是 Unreal Engine 的一个生命周期函数，它会在 Actor 的所有组件都确认创建并初始化完成之后调用。在这里进行事件绑定是最安全、最可靠的时机，
	可以确保 MeshComp 指针是有效且准备好的。*/
}
void ASExplosiveBarrel::OnActionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();//这是爆炸的​​具体执行逻辑​​。ForceComp 是一个径向力组件（URadialForceComponent），FireImpulse() 函数会命令它立即在其位置爆发一个强大的物理力。
	UE_LOG(LogTemp, Log, TEXT("onActionhIT in Exposive Barrel"));

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s,at game time:%f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	FString CombinedString = FString::Printf(TEXT("Hit at location:%s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	/*UE_LOG 宏的第一个参数是​​日志类别（Log Category）​​，可以在源文件中自定义日志类型
	* // 在头文件中声明日志类别
		DECLARE_LOG_CATEGORY_EXTERN(LogMyCategory, Log, All);
		LogMyCategory：自定义类别名称（建议使用模块名前缀）
		Log：默认日志级别
		All：编译时包含所有级别的日志
		UE自带的日志类型
	类别名称	用途
	LogTemp	​​临时日志​​（无需声明即可使用）
	LogCore	核心系统日志	
	LogNet	网络相关日志
	LogPhysics	物理系统日志
	LogAI	人工智能日志
	LogGameplay	游戏逻辑日志*/
}
// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;//设定根组件

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("Forcecomp");//该组件可以在一个球形范围内施加物理冲击力。
	ForceComp->SetupAttachment(MeshComp);//将径向力组件​​附加​​到网格体组件上。这意味着力的中心点将始终与网格体的位置保持一致。
	ForceComp->SetAutoActivate(false);//设置这个力场不会自动爆发

	ForceComp->Radius = 750.1f;//力的作用半径（750单位）
	ForceComp->ImpulseStrength = 2500.0f;//冲击力强度（2500单位）。值越大，物体被炸飞得越远。
	ForceComp->bImpulseVelChange = true;//设为 true 表示​​忽略物体质量​​，直接赋予速度变化。这能确保无论物体多重，都能被炸飞，创造出更夸张、更游戏性的效果。

	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);//指定力场会影响哪些类型的物体​​。

	//MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);//绑定事件委托​​，这行代码建立了“​​当桶被击中时，就调用 OnActorHit 函数来处理这次击中​​”的响应机制。
}


