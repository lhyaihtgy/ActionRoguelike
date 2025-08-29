// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChestActor.h"



// Sets default values
ASItemChestActor::ASItemChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);//LidMesh​​附加​​到 BaseMesh上。SetupAttachment方法建立了父子层级关系。
	TargetPitch = 110;
}

//这个函数的作用是当玩家触发宝箱后让宝箱的上半部分移动110度
void ASItemChestActor::Interact_Implementation(APawn* instigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));//让宝箱顶部旋转110°
}


// Called when the game starts or when spawned
void ASItemChestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

