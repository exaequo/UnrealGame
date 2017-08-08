// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MyProject2.h"
#include "AllmightyMaster.h"
#include "MyProject2Ball.h"

AMyProject2Ball::AMyProject2Ball()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	//Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow0"));
	Arrow->SetStaticMesh(BallMesh.Object);
	Arrow->SetSimulatePhysics(false);
	Arrow->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Arrow->SetVisibility(false);

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;//false
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(0.f, -45.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = true;//false
	SpringArm->CameraLagSpeed = 3.f;


	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; 

	FTriggerSphereSize = 10.f;
	TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyProject2Ball::OnSphereOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &AMyProject2Ball::OnSphereOverlapEnd);
	TriggerComponent->SetRelativeScale3D(FVector{ FTriggerSphereSize, FTriggerSphereSize, FTriggerSphereSize });

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	InAirForce = 10.f;
	MaxBallVelocity = 10.f;
	MaxBallAngularVelocity = 10.f;
	CameraRotationSensitivity = 10.f;
	bCanJump = true; // Start being able to jump
	bIsOnSwitch = false; //Start while not being in some switch
}

void AMyProject2Ball::BeginPlay() 
{
	AAllmightyMaster::AddBall(this);

	SetArrowRotation();

	TriggerComponent->SetRelativeScale3D(FVector{ FTriggerSphereSize, FTriggerSphereSize, FTriggerSphereSize });

	Ball->SetPhysicsMaxAngularVelocity(MaxBallAngularVelocity);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("AngularVelocityCHANGED"));
}

void AMyProject2Ball::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveRight", this, &AMyProject2Ball::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &AMyProject2Ball::MoveForward);
	InputComponent->BindAxis("Rotate", this, &AMyProject2Ball::RotateCamera);
	InputComponent->BindAxis("RotatePitch", this, &AMyProject2Ball::PitchCamera);
	
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyProject2Ball::Jump);
	InputComponent->BindAction("Possess", IE_Pressed, this, &AMyProject2Ball::CheckForPossession);
}

void AMyProject2Ball::OnSphereOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//AMyProject2Ball *OtherBall = dynamic_cast<AMyProject2Ball*>(OtherActor);
	//if (OtherBall != nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("OVERLAPPING"));
	//	BallInside = OtherBall;
	//	//OtherBall->Ball->bRenderCustomDepth = true;
	//}	
}

void AMyProject2Ball::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AMyProject2Ball *OtherBall = dynamic_cast<AMyProject2Ball*>(OtherActor);
	//if (OtherBall != nullptr)
	//{
	//	//OtherBall->Ball->bRenderCustomDepth = false;
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("ENDING OVERLAP"));
	//	BallInside = nullptr;
	//}
}

void AMyProject2Ball::RotateCamera(float Val)
{
	FRotator rot = SpringArm->GetComponentRotation();

	rot.Yaw += Val * GetWorld()->DeltaTimeSeconds * CameraRotationSensitivity;

	SpringArm->SetRelativeRotation(rot);

	SetArrowRotation();
}

void AMyProject2Ball::PitchCamera(float Val)
{
	FRotator rot = SpringArm->GetComponentRotation();

	rot.Pitch =  FMath::Clamp( rot.Pitch + Val * GetWorld()->DeltaTimeSeconds * CameraRotationSensitivity, -80.f, -10.f);

	SpringArm->SetRelativeRotation(rot);
}

void AMyProject2Ball::CheckForPossession()
{
	AMyProject2Ball* BallInside = AAllmightyMaster::GetNextBall(this);
	if (BallInside != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("POSSESSING"));
		AController* controller = GetController();
		controller->UnPossess();
		controller->Possess(BallInside);
		DrawPossessionLine(GetActorLocation(), BallInside->GetActorLocation());
		
		BallInside->Ball->bRenderCustomDepth = true;
		Ball->bRenderCustomDepth = false;
		BallInside->GetSpringArm()->SetRelativeRotation(SpringArm->GetComponentRotation());

		if (bIsOnSwitch)
		{
			Ball->SetPhysicsLinearVelocity(FVector::ZeroVector);
			Ball->SetPhysicsAngularVelocity(FVector::ZeroVector);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("CANNOT POSSESS"));
	}
}

bool AMyProject2Ball::RaycastWallCheck(const FVector& direction) const
{
	bool value = false;
	
	FHitResult* HitResult = new FHitResult();
	float scale = Ball->GetComponentScale().X;

	FVector Start = Ball->GetComponentLocation(); //this->GetActorLocation();
	FVector End = Start + direction.ProjectOnToNormal(direction) * scale * 0.55f;

	FCollisionQueryParams* CQP = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, Start, End, ECC_Visibility, *CQP))
	{
		value = true;
	}

	return value;
}



void AMyProject2Ball::CheckMaxSpeed()
{
	FVector Vel = GetVelocity();
	Vel.Z = 0.f;

	if (FMath::Abs(Vel.Size()) > MaxBallVelocity)
	{
		Vel.Normalize();
		Vel *= MaxBallVelocity;
		Vel.Z = GetVelocity().Z;
		Ball->SetPhysicsLinearVelocity(Vel);
	}
}

void AMyProject2Ball::SetArrowRotation()
{
	FRotator Rot{ 0.f,0.f,0.f };
	Rot.Yaw = SpringArm->GetComponentRotation().Yaw;
	Arrow->SetRelativeRotation(Rot);
}

void AMyProject2Ball::DrawPossessionLine(const FVector firstPoint, const FVector secondPoint) const
{
	DrawDebugLine(GetWorld(), firstPoint, secondPoint, FColor{ 255, 255 ,255 }, false, 1.0f, (uint8)'\000', 5.f);
}

void AMyProject2Ball::Move(const FVector direction, const FVector perpendicular, const float Val, const FVector rawDirection, bool withTraversalHelp, float ReverseVal)
{
	const FVector Torque = perpendicular * (-1.f) * RollTorque * Val * ReverseVal;

	if (bCanJump)
	{
		if (withTraversalHelp)
		{
			FVector velocity = GetVelocity();

			if ((Val > 0 && FVector::DotProduct(velocity.ProjectOnToNormal(velocity), direction) < -0.5f) ||
				(Val < 0 && FVector::DotProduct(velocity.ProjectOnToNormal(velocity), -direction) < -0.5f))
			{
				FVector zeroedVelocity{ velocity.X, velocity.Y, velocity.Z };
				zeroedVelocity *= (FVector{ 1.f, 1.f, 1.f } -rawDirection);

				Ball->SetPhysicsLinearVelocity(FMath::Lerp(velocity, zeroedVelocity, GetWorld()->GetDeltaSeconds() * 10.f)); //FVector{ velocity.X, 0.f, velocity.Z }

			}
		}
	}
	else
	{
		if (RaycastWallCheck(direction * Val))
		{
			Ball->AddForce(direction * Val * InAirForce * GetWorld()->GetDeltaSeconds(), NAME_None, true);
		}
	}

	Ball->AddTorque(Torque, NAME_None, true);

	CheckMaxSpeed();
}

void AMyProject2Ball::MoveRight(float Val)
{
	
	Move(Arrow->GetRightVector(), Arrow->GetForwardVector(), Val, FVector::RightVector);

}

void AMyProject2Ball::MoveForward(float Val)
{

	Move(Arrow->GetForwardVector(), Arrow->GetRightVector(), Val, FVector::ForwardVector, true, -1.f);

}

void AMyProject2Ball::Jump()
{
	if(bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse, NAME_None, true);
		bCanJump = false;
	}
}

void AMyProject2Ball::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(FVector::DotProduct(HitNormal, FVector::UpVector) >= 0.5f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("YES"));
		bCanJump = true;
	}
	
}
