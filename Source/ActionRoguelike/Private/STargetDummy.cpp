// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);//将这个组件中的血量变化函数绑定到血量buff组件的激发函数中，当血量变化的时候
	//这个组件中的onhealthchanged函数就会触发

}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)//只处理攻击生效的时候
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetTimeSeconds());//将材质参数中的TimeToHit设置未当前的游戏时间
	}
}
