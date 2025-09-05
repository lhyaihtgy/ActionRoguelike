// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementcomponent.h"
#include "SAttributeComponent.h"
#include "particles/ParticleSystemComponent.h"
// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");//实例化球形碰撞体组件
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);//这告诉碰撞系统，这个飞弹属于“​​动态世界物体​​”类别。这与静态环境（如墙壁 ECC_WorldStatic）或角色（ECC_Pawn）区分开来，表明它是可移动、可交互的。
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);//设置该碰撞体​​默认忽略所有类型的碰撞通道​​。ECR_Ignore意味着它不会阻挡任何物体，也不会触发与任何物体的重叠事件。
	////目的​​：这是一张“白纸”策略。先清空所有默认响应，为后面精细化的、针对特定通道的设置做准备。这确保了飞弹​​默认情况下是隐形的、不会与任何东西交互的​​。
	//SphereComp->SetCollisionResponseToChannels(ECR_Ignore);//忽略所有类型的碰撞通道，不与任何其他物体发生任何形式的交互。
	///*以上设置碰撞的代码都可以在ue中进行设置成为一个配置文件，然后使用下面的代码读取配置文件，此时就不需要上面的设置碰撞的代码了*/
	SphereComp->SetCollisionProfileName("Projectile");//直接应用一个名为 "Projectile"的​​预设配置​​。这个预设是我在引擎中设置好的碰撞行为，通常已经配置好了抛射物应有的碰撞行为（例如，忽略其他抛射物，与Pawn和WorldStatic发生重叠或阻挡）。
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);//现在增加了一个功能是当飞弹重叠的时候需要让角色的血量减少，所以这里需要让血量组件绑定到球形碰撞组件上
	//OnComponentBeginOverlap是一个​​事件​​。当该碰撞组件​​开始​​与任何其他符合条件的组件发生重叠（Overlap）时，OnActorOverlap就会被引擎自动触发。
	RootComponent = SphereComp;//将刚创建的 SphereComp设置为Actor的 ​​RootComponent​​（根组件）。

	EffectComp = CreateAbstractDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);//将粒子组件附加到球形组件上

	MovementComp = CreateAbstractDefaultSubobject<UProjectileMovementComponent>("MovementComp");//抛射物移动组件实例化
	MovementComp->InitialSpeed = 1000.0f;//确定初始速度为1000
	MovementComp->bRotationFollowsVelocity = true;//当设置为 true时，抛射物​​自身的旋转会与其飞行方向保持一致​​。就像导弹或箭在飞行中会调整自身角度，始终指向其前进的方向。
	MovementComp->bInitialVelocityInLocalSpace = true;//当设置为 true时，InitialSpeed施加的速度方向是​​相对于抛射物自身的坐标系​​（例如，它的正前方）。这确保了无论Actor如何旋转，抛射物都会朝它“鼻子”所指的方向（即其正前方）
	// 发射，这是最直观的行为。
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);

			Destroy();//这个魔法飞弹到这里就完成了自己的任务，销毁
		}
	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

