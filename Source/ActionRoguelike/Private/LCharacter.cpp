// Fill out your copyright notice in the Description page of Project Settings.


#include "LCharacter.h"
#include "GameFramework/SpringArmComponent.h" // �ṩ USpringArmComponent ��Ķ���
#include "Camera/CameraComponent.h"          // �ṩ UCameraComponent ��Ķ���


// Sets default values
ALCharacter::ALCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmCom");//ʵ����һ�����ɱ۵����
	SpringArmComp->SetupAttachment(RootComponent);//�����ɱ۵�������õ�Ĭ�ϵĽ���������
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CamperCom"); //ʵ����һ������������
	CameraComp->SetupAttachment(SpringArmComp);//���������װ�����ɱ�����
	//����Ĳ������root��һ��ģ����������һ�������ĵ����˳���Ϸ���ӽ�

	 
}

void ALCharacter::MoveForward(float value)//�������ֵΪ1��ʱ�����ǰ�������������Ǻ���
{
	AddMovementInput(GetActorForwardVector(), value);//����˵����������ǿ��ƽ�ɫ�ŵĺ���,�����ƶ�������ȥ,Get���ص���һ��������������add����ǰ�����ڵ���������ϵ

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
	//����Ҫ���������ɫ���������
	PlayerInputComponent->BindAxis("MoveForward", this, &ALCharacter::MoveForward);//��������move����ַ�����ʱ���ȥִ�����������
	//������ʵ��ת��
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);//Add����������Ϊ��Pawn�����ŵĿ�������Controller�����һ����ƫ������Yaw����ת���롣����ʵ��ת��Ĳ���?

}

