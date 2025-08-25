// Fill out your copyright notice in the Description page of Project Settings.


#include "LCharacter.h"
#include "GameFramework/SpringArmComponent.h" // 提供 USpringArmComponent 类的定义
#include "Camera/CameraComponent.h"          // 提供 UCameraComponent 类的定义


// Sets default values
ALCharacter::ALCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmCom");//实例化一个弹簧臂的组件
	SpringArmComp->SetupAttachment(RootComponent);//将弹簧臂的组件设置到默认的胶囊体上面
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CamperCom"); //实例化一个摄像机的组件
	CameraComp->SetupAttachment(SpringArmComp);//将摄像机安装到弹簧臂上面
	//上面的步骤如果root是一个模型这就完成了一个基本的第三人称游戏的视角

	 
}

void ALCharacter::MoveForward(float value)//当输入的值为1的时候代表前进，否则代表的是后退
{
	AddMovementInput(GetActorForwardVector(), value);//简单来说这个函数就是控制角色脚的函数,让其移动到哪里去,Get返回的是一个数组向量告诉add函数前方所在的向量坐标系

}

// Called when the game starts or when spawned
void ALCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//下面要设置这个角色的输入组件
	PlayerInputComponent->BindAxis("MoveForward", this, &ALCharacter::MoveForward);//当触发了move这个字符串的时候会去执行这个函数，
	//下面来实现转弯
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);//Add函数的作用为该Pawn所附着的控制器（Controller）添加一个“偏航”（Yaw）旋转输入。用以实现转向的操作?

}

