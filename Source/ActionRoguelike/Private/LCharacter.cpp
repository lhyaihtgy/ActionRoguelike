// Fill out your copyright notice in the Description page of Project Settings.

#include "LCharacter.h"
#include "GameFramework/SpringArmComponent.h" // 提供 USpringArmComponent 类的定义
#include "Camera/CameraComponent.h"          // 提供 UCameraComponent 类的定义
#include "GameFramework/characterMovementComponent.h"
#include "SAttributeComponent.h"
#include "SDashProjectile.h"

// Sets default values
ALCharacter::ALCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmCom");//实例化一个弹簧臂的组件
	SpringArmComp->bUsePawnControlRotation = true;//允许玩家用鼠标​​自由控制视角​​旋转，观察四周。
	SpringArmComp->SetupAttachment(RootComponent);//将弹簧臂的组件设置到默认的胶囊体上面

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CamperCom"); //实例化一个摄像机的组件
	CameraComp->SetupAttachment(SpringArmComp);//将摄像机安装到弹簧臂上面
	//上面的步骤如果root是一个模型这就完成了一个基本的第三人称游戏的视角

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionCom");//实例化交互功能的类

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributesCom");

	GetCharacterMovement()->bOrientRotationToMovement = true;//让摄像机的旋转通过弹簧臂完全跟随控制器的输入。这意味着我可以​​自由地用鼠标环顾四周​​，摄像机镜头会左右、上下转动。
	bUseControllerRotationYaw = false;//这个代码和上一行代码相互帮助，这个代码的作用是解耦角色的身体朝向和摄像机的朝向，让其实现黑魂那种摄像机往后依旧可以往前的操作视角

	 
}

//处理人物前进和后退的代码
void ALCharacter::MoveForward(float value)//当输入的值为1的时候代表前进，否则代表的是后退
{
	//新添加的修改是，当我按下前进的时候我的视角在哪边角色就要往这个方向前进
	FRotator controloat = GetControlRotation();//它获取的是​​玩家控制器（Player Controller）的旋转​​。这个旋转直接由玩家的鼠标输入控制，代表了​​摄像机的朝向​​。将这摄像机的旋转值存储在一个名为 controlRot的 FRotator变量中。
	//FRotator包含 Pitch（上下俯仰）、Yaw（左右偏航）、Roll（翻滚）三个分量。
	controloat.Pitch = 0.0f;//将俯仰角（上下看的角度）强制设置为0。这意味着​​忽略摄像机的上下倾斜​​。无论玩家是看着天空还是地面，移动都只发生在水平面上
	controloat.Roll = 0.0f;//将翻滚角（侧滚角度）强制设置为0。这个在大多数情况下本来就是0，但这是一个良好的习惯，确保旋转纯净。
	AddMovementInput(controloat.Vector(), value);//第二版实现了视角往哪边，按下前进后往那边前进
	//AddMovementInput(GetActorForwardVector(), value);//第一版简单来说这个函数就是控制角色脚的函数,让其移动到哪里去,Get返回的是一个数组向量告诉add函数前方所在的向量坐标系，无法实现摄像头转向哪边我就往哪边前进
}

//处理人物左右移动的代码
void ALCharacter::MoveRight(float value)//第一版本和前进后退函数是一致的
{
	//第二版本能够让左右移动也和前进一样向摄像机转动的方向前进
	FRotator controloat = GetControlRotation();
	controloat.Pitch = 0.0f;
	controloat.Roll = 0.0f;
	//这三行和前进的代码是一样的，目的​​：确保移动方向是纯粹的水平方向，避免角色因为玩家抬头或低头而向斜方向移动。
	
	//下面需要获取往右边移动时的向量
	FVector rightvector = FRotationMatrix(controloat).GetScaledAxis(EAxis::Y);
	AddMovementInput(rightvector, value);//开始进行旋转，第二版解决的是，第一版左转和右转会围绕着一个点进行圈行旋转的问题，第二版按下a和d按键后会向着摄像机的左边和右边进行移动

	//AddMovementInput(GetActorRightVector(), value);//第一版本和前进后退函数是一致的
}

/*
*该函数的主要逻辑
*获取发射位置​​（手部插槽 "Muzzle_01"）。
​​*检测目标点​​（摄像机前方 5000 单位或命中点）。
​​*计算投射物朝向​​（使投射物飞向目标点）。
​​*生成投射物​​（使用 SpawnActor实例化投射物）。
*/
void ALCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))//确保施法者指针存在
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");//得到该施法者手部的坐标


		//设置投射物的生成参数
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//即使生成位置被阻挡，也会强制生成投射物（避免因碰撞问题导致投射物无法生成）。
		SpawnParams.Instigator = this;//设置当前角色为投射物的 Instigator（施法者），用于后续伤害计算（如 UGameplayStatics::ApplyDamage）。

		//设置碰撞检测参数​
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);//// 定义一个半径为 20 的球形碰撞检测范围

		FCollisionQueryParams Parms;
		Parms.AddIgnoredActor(this);//// 忽略当前角色，避免投射物误伤自己

		FCollisionObjectQueryParams ObjParms;
		ObjParms.AddObjectTypesToQuery(ECC_WorldDynamic);// 检测动态物体（如可移动的 Actor）
		ObjParms.AddObjectTypesToQuery(ECC_WorldStatic);// 检测静态物体（如墙壁、地面）
		ObjParms.AddObjectTypesToQuery(ECC_Pawn);// 检测其他角色（Pawn）

		//计算投射方向（射线检测）​​
		FVector TraceStart = CameraComp->GetComponentLocation();// 射线起点（摄像机位置）

		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);//// 射线终点（摄像机前方 5000 单位）

		FHitResult Hit;//储存命中心下

		// 执行球形射线检测
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParms, Shape, Parms))
		{
			TraceEnd = Hit.ImpactPoint;// 如果命中物体，则投射物朝向命中点
		}

		//计算投射物旋转
		//TraceEnd - HandLocation​​：计算从手部位置到目标点的方向向量。FRotationMatrix::MakeFromX​​：生成一个旋转矩阵，使 X 轴朝向目标方向。​​作用​​：确保投射物朝向正确的飞行方向。
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		//生成投射物​
		/*​​FTransform​​：定义投射物的初始位置和旋转。SpawnActor​​：在 HandLocation位置生成投射物，并设置其旋转朝向目标方向。*/
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}
void ALCharacter::Dash()
{
	PlayAnimMontage(AttackAni);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ALCharacter::Dash_TimeElapsed, AttackAniDelay);
}
void ALCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}
// Called when the game starts or when spawned
void ALCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//生成魔法飞弹抛射物的代码
void ALCharacter::PrimaryAttack()
{
	//这里
	PlayAnimMontage(AttackAni);//这个函数能够为我们播放一个攻击动画,但是因为飞弹的生成是和手臂相关的，导致当我按下攻击的时候，飞弹已经生成但是攻击动画还在播出导致出现了问题
	//，其中一个解决方法是当动画准备好之后去通知生成事件，这里暂不做考虑
	//另外一个就是延迟生成的时间让其在攻击动画完成的前一瞬间在手臂生成飞弹
	//这里加入计时器延时
	GetWorldTimerManager().SetTimer(FTimerHandle_PrimaryAttack, this, &ALCharacter::PrimaryAttack_TimeElapsed,0.2f);
	//GetWorldTimerManager().ClearTimer(FTimerHandle_PrimaryAttack);//这个代码是清除这个计时器，如果在你的角色死亡之后不再需要这个计时器相应了
	//以下生成飞弹的代码，在需要匹配攻击动画的版本需要将下面的逻辑放入到计时器的回调函数中，方便在计时器结束后进行回调
	///*获取角色骨骼中的某一个位置让魔法飞弹从这个位置开始进行生成*/
	//FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);//这个代码让魔法飞弹从手臂出进行生成,第二版的修正
	///*现在的第一版代码虽然生成了一个飞弹但是并不是从手臂上生成的需要让其在手臂上进行生成*/
	////FTransform SpawnTM = FTransform(GetControlRotation(), GetActorLocation());//第一版并不是从手臂的骨骼上生成的魔法飞弹创建了一个变换定义了新物体在游戏世界中的位置和旋转
	////GetControlRotation:获取当前角色（ASCharacter）在世界空间中的坐标位置。这将是抛射物生成的​​起点​​
	////​​GetControlRotation()​​：获取玩家控制器（Player Controller）的旋转。这个旋转直接由玩家的鼠标控制，代表了​​摄像机的朝向​​。这将是抛射物生成的​​初始方向​​。

	//FActorSpawnParameters SpawnParams;//创建了一个（生成Actor）的结构体，在生成 Actor 时，通过设置其成员变量，向引擎传递额外的生成信息，以自定义生成过程。

	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//后面的参数是一个枚举值代表总是生成，无视碰撞,这个函数设置了生成是遇到碰撞的处理方式
	////后面的参数保证了即使存在碰撞也能够生成，后裔的移动和碰撞再由抛射物本身的组件来处理


	//GetWorld()->SpawnActor<AActor>(ProjectilesClass, SpawnTM, SpawnParams);
	///*​​GetWorld()​​：获取当前的游戏世界对象，生成Actor需要这个上下文。
	//​​SpawnActor<AActor>​​：这是一个模板函数，用于在游戏世界中生成一个新的Actor。<AActor>是生成的Actor类型，这里使用基类，意味着它可以生成任何继承自 AActor的类。
	//SpawnTM​​：之前创建的变换，决定了新Actor的位置和旋转。
	//SpawnParams​​：之前设置的生成参数，决定了碰撞处理方式。*/
}

void ALCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ALCharacter::PrimaryAttack_TimeElapsed()
{
	//if (ensureAlways(ProjectilesClass))////确保在UE4编译器中Pro类已经被设置了而不是一个空指针，ensure只会在第一次运行时触发，之后不会触发，如果要一直触发需要使用ensurealways
	//	//如果这里使用check那么在判断pro为空程序会直接崩溃，而sure只会记录错误然后继续执行
	//{
	//	/*获取角色骨骼中的某一个位置让魔法飞弹从这个位置开始进行生成*/
	//	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	//	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);//这个代码让魔法飞弹从手臂出进行生成,第二版的修正
	//	/*现在的第一版代码虽然生成了一个飞弹但是并不是从手臂上生成的需要让其在手臂上进行生成*/
	//	//FTransform SpawnTM = FTransform(GetControlRotation(), GetActorLocation());//第一版并不是从手臂的骨骼上生成的魔法飞弹创建了一个变换定义了新物体在游戏世界中的位置和旋转
	//	//GetControlRotation:获取当前角色（ASCharacter）在世界空间中的坐标位置。这将是抛射物生成的​​起点​​
	//	//​​GetControlRotation()​​：获取玩家控制器（Player Controller）的旋转。这个旋转直接由玩家的鼠标控制，代表了​​摄像机的朝向​​。这将是抛射物生成的​​初始方向​​。

	//	FActorSpawnParameters SpawnParams;//创建了一个（生成Actor）的结构体，在生成 Actor 时，通过设置其成员变量，向引擎传递额外的生成信息，以自定义生成过程。

	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//后面的参数是一个枚举值代表总是生成，无视碰撞,这个函数设置了生成是遇到碰撞的处理方式
	//	//后面的参数保证了即使存在碰撞也能够生成，后裔的移动和碰撞再由抛射物本身的组件来处理
	//	SpawnParams.Instigator = this;//将生成魔法飞弹的类传递给魔法飞弹让魔法飞弹能够忽略对发起者的碰撞检测

	//	GetWorld()->SpawnActor<AActor>(ProjectilesClass, SpawnTM, SpawnParams);
	//	/*​​GetWorld()​​：获取当前的游戏世界对象，生成Actor需要这个上下文。
	//	​​SpawnActor<AActor>​​：这是一个模板函数，用于在游戏世界中生成一个新的Actor。<AActor>是生成的Actor类型，这里使用基类，意味着它可以生成任何继承自 AActor的类。
	//	SpawnTM​​：之前创建的变换，决定了新Actor的位置和旋转。
	//	SpawnParams​​：之前设置的生成参数，决定了碰撞处理方式。*/
	//}
	//以上为第一个版本的生成魔法飞弹的版本，这个版本将所有要做的事情都放在了这个函数中，从头到尾，但是这个逻辑在其他的生成类中依旧会使用，所以这里将这些生成逻辑集合成为了一个函数模板
	//这个函数直接去调用这个函数模板去进行实例化即可
	SpawnProjectile(ProjectilesClass);
}

void ALCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAni);//播放前摇动画
	GetWorldTimerManager().SetTimer(TimeHandle_BlackHole, this, &ALCharacter::BlackholeAttack_TimeElapsed, AttackAniDelay);
}

void ALCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
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
	//前后
	PlayerInputComponent->BindAxis("MoveForward", this, &ALCharacter::MoveForward);//当触发了move这个字符串的时候会去执行这个函数，
	//左右
	PlayerInputComponent->BindAxis("MoveRight",this,&ALCharacter::MoveRight);
	//下面来实现转弯
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);//Add函数的作用为该Pawn所附着的控制器（Controller）添加一个“偏航”（Yaw）旋转输入。用以实现转向的操作?
	//实现让镜头能够上下切换
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);//这个函数用以实现让摄像机能够上下旋转
	//实现当玩家按下某一个被我映射的按键之后玩家会释放出魔法飞弹
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ALCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ALCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ALCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ALCharacter::Dash);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALCharacter::Jump);
}

