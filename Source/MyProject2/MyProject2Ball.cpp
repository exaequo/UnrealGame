// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MyProject2.h"
#include "AllmightyMaster.h"
#include "Public/HUDController.h"
#include "Public/GrabableComponent.h"
#include "MyProject2Ball.h"

float AMyProject2Ball::TimeSlowTime{ 2.f };
float AMyProject2Ball::TimeSlowSlowMultiplier{ 0.3f };

AMyProject2Ball::AMyProject2Ball()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = RootComp;

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->SetupAttachment(RootComponent);

	

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow0"));
	Arrow->SetStaticMesh(BallMesh.Object);
	Arrow->SetSimulatePhysics(false);
	Arrow->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Arrow->SetVisibility(false);

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(Ball);
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

	TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerComponent->AttachToComponent(Ball, FAttachmentTransformRules::KeepRelativeTransform);

	HUDController = CreateDefaultSubobject<UHUDController>("HUDController");

	AdditionalGravity = FVector::ZeroVector;
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	InAirForce = 10.f;
	MaxBallVelocity = 10.f;
	RopeStrength = 100.f;
	RopeLength = 1000.f;
	RopeReloadTime = 2.f;
	bCanRope = true;
	MaxBallAngularVelocity = 10.f;
	CameraRotationSensitivity = 10.f;
	RopeZBlindSpotDistance = 100.f;
	RopeFreeObjectMovementMultplier = 1.0f;
	bCanJump = true; // Start being able to jump
	bIsOnSwitch = false; //Start while not being in some switch
}

void AMyProject2Ball::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Ball->AddForce(AdditionalGravity * DeltaSeconds * Ball->GetMass());

	CameraPointRaycast();

	float SizeMultiplier = 1.f - (FVector::Dist(LocationToGrab, Camera->GetComponentLocation()) / RopeLength);
	
	if (CheckIfNearNextFlyLocation())
	{
		ReloadRope();
		CurrentFlyLocation = FVector::ZeroVector;
		CurrentFlyLocationDistance = 0;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("NEAR NEXT FLY LOC"));
	}

	HUDController->UpdatePointerImage(dynamic_cast<APlayerController*>(GetController()), LocationToGrab, SizeMultiplier );
}

void AMyProject2Ball::BeginPlay()
{
	Super::BeginPlay();

	AAllmightyMaster::AddBall(this);

	SetArrowRotation();

	Ball->SetPhysicsMaxAngularVelocity(MaxBallAngularVelocity);
	DefaultBallVelocity = MaxBallVelocity;
	DefaultAdditionalGravity = AdditionalGravity;

	HUDController->Init(this);
}

void AMyProject2Ball::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

FVector AMyProject2Ball::GetPositionFromRaycast(const FHitResult & HitResult) const
{
	FVector Result{ HitResult.ImpactPoint };
	
	FBox Box = HitResult.GetComponent()->Bounds.GetBox();
	FVector BoxCenter = Box.GetCenter();
	FVector BoxExtent = Box.GetExtent();

	float MaxZPos = HitResult.ImpactPoint.Z;

	for (auto PointMapping : AAllmightyMaster::BoundsPointMapping)
	{
		float PointZ = (BoxCenter + PointMapping * BoxExtent).Z;
		MaxZPos = (PointZ > MaxZPos) ? PointZ : MaxZPos;
	}

	Result.Z = MaxZPos;
	return Result;
}

void AMyProject2Ball::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up restart key
	FInputActionBinding ActionBinding{ "Restart", IE_Pressed };
	FInputAxisBinding AxisBinding{ "MoveRight" };

	ActionBinding.ActionDelegate.GetDelegateForManualSet().BindLambda(
		[this]() { AAllmightyMaster::RestartLevel(dynamic_cast<APlayerController*> (GetController())); }
	);
	InputComponent->AddActionBinding(ActionBinding);

	// set up jump key
	ActionBinding.ActionName = "Jump";
	ActionBinding.ActionDelegate.GetDelegateForManualSet().BindLambda(
		[this]() {
			if (bCanJump){
				const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
				Ball->AddImpulse(Impulse, NAME_None, true);
				bCanJump = false;
			}
	});
	InputComponent->AddActionBinding(ActionBinding);

	// set up rope key
	ActionBinding.ActionName = "Rope";
	ActionBinding.ActionDelegate.GetDelegateForManualSet().BindLambda(
		[this]() {
			if (bCanRope)
			{
				if (LocationToGrab != AGrabableObject::NothingToGrab)
				{
					// HERE
					Rope(Ball->GetComponentLocation(), LocationToGrab);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("ROPE DIDNT HIT"));
				}

				PrematureSlowTimeStop();
			}
	});
	InputComponent->AddActionBinding(ActionBinding);

	// set up move right axis
	AxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda(
		[this](float Val) {Move(Arrow->GetRightVector(), Arrow->GetForwardVector(), Val, FVector::RightVector);
	});
	InputComponent->AxisBindings.Add(AxisBinding);

	// set up move forward axis
	AxisBinding.AxisName = "MoveForward";
	AxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda(
		[this](float Val) {Move(Arrow->GetForwardVector(), Arrow->GetRightVector(), Val, FVector::ForwardVector, true, -1.f);
	});
	InputComponent->AxisBindings.Add(AxisBinding);

	InputComponent->BindAxis("Rotate", this, &AMyProject2Ball::RotateCamera);
	InputComponent->BindAxis("RotatePitch", this, &AMyProject2Ball::PitchCamera);
	
	//InputComponent->BindAction("Jump", IE_Pressed, this, &AMyProject2Ball::Jump);
	InputComponent->BindAction("Possess", IE_Pressed, this, &AMyProject2Ball::CheckForPossession);
}

void AMyProject2Ball::RotateCamera(float Val)
{
	FRotator rot = SpringArm->GetComponentRotation();

	rot.Yaw += Val * GetWorld()->DeltaTimeSeconds * CameraRotationSensitivity * (1.f / UGameplayStatics::GetGlobalTimeDilation(GetWorld()));

	SpringArm->SetRelativeRotation(rot);

	SetArrowRotation();
}

void AMyProject2Ball::PitchCamera(float Val)
{
	FRotator rot = SpringArm->GetComponentRotation();

	rot.Pitch =  FMath::Clamp( rot.Pitch + Val * GetWorld()->DeltaTimeSeconds * CameraRotationSensitivity  * (1.f / UGameplayStatics::GetGlobalTimeDilation(GetWorld())), -80.f, -5.f);

	SpringArm->SetRelativeRotation(rot);
}

void AMyProject2Ball::CheckForPossession()
{
	AMyProject2Ball* BallInside = AAllmightyMaster::GetNextBall(this);
	if (BallInside)
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
	
	FHitResult HitResult{};
	float scale = Ball->GetComponentScale().X;

	FVector Start = Ball->GetComponentLocation();
	FVector End = Start + direction.ProjectOnToNormal(direction) * scale * 0.55f;

	FCollisionQueryParams CQP{};

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CQP))
	{
		value = true;
	}

	return value;
}



void AMyProject2Ball::CheckMaxSpeed()
{
	FVector Vel = Ball->GetPhysicsLinearVelocity();

	Vel.Z = 0.f;

	if (FMath::Abs(Vel.Size()) > MaxBallVelocity)
	{
		Vel.Normalize();
		Vel *= MaxBallVelocity;
		Vel.Z = Ball->GetPhysicsLinearVelocity().Z;
		Ball->SetPhysicsLinearVelocity(Vel);
	}
}

void AMyProject2Ball::SetArrowRotation()
{
	FRotator Rot{ 0.f,0.f,0.f };
	Rot.Yaw = SpringArm->GetComponentRotation().Yaw;
	Arrow->SetRelativeRotation(Rot);
}

void AMyProject2Ball::CameraPointRaycast()
{
	FHitResult HitResult{};

	FVector Direction = SpringArm->GetForwardVector();
	Direction += FVector::UpVector * CameraRaycastUpModifier;
	Direction.Normalize();

	float DistanceFromPlayer = FVector::Dist(Ball->GetComponentLocation(), Camera->GetComponentLocation());
	FVector Start = Camera->GetComponentLocation() + Direction * DistanceFromPlayer; //this->GetActorLocation();
	FVector End = Start + RopeLength * Direction;

	FCollisionQueryParams CQP{};

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera, CQP))
	{
		
		AGrabableObject* Grabable = Cast<AGrabableObject>(HitResult.GetActor());

		//TArray<USceneComponent*> GrabableComponents{};
		//HitResult.GetComponent()->GetChildrenComponents(false, GrabableComponents);
		
		//TArray<UActorComponent*> GrabableComponent = HitResult.GetActor()->GetComponentsByClass(UGrabableComponent::StaticClass());
		/*for (USceneComponent* Child : GrabableComponents)
		{
			UGrabableComponent* GrabableComponent = Cast<UGrabableComponent>(Child);
			if (GrabableComponent)
			{
				CanGrab = GrabableComponent->bCanGrab;
				break;
			}
		}*/

		FVector Point = GetPositionFromRaycast(HitResult);
		FVector PointXY = Point - Ball->GetComponentLocation();
		float PointZ = PointXY.Z;
		PointXY.Z = 0;

		if (PointXY.Size() > RopeXYBlindSpotDistance && PointXY.Size() > FMath::Abs(PointZ)) //in order to get rid of the jumping high up with rope
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("S: %f, Dist: %f"), PointXY.Size(), RopeXYBlindSpotDistance));

			ObjectToGrab = (Grabable) ? Grabable : nullptr;

			LocationToGrab = Point + RopeUpOffset * FVector::UpVector;
		}
		else
		{
			ObjectToGrab = nullptr;

			LocationToGrab = AGrabableObject::NothingToGrab;
		}

		
		
	}
	else
	{
		LocationToGrab = AGrabableObject::NothingToGrab;
		
	}

	if (LocationToGrab != AGrabableObject::NothingToGrab && 
		LocationToGrab.Z > Ball->GetComponentLocation().Z + RopeZBlindSpotDistance)
	{
		ObjectToGrab = nullptr;

		LocationToGrab = AGrabableObject::NothingToGrab;
	}
}


void AMyProject2Ball::DrawPossessionLine(const FVector& firstPoint, const FVector& secondPoint) const
{
	DrawDebugLine(GetWorld(), firstPoint, secondPoint, FColor{ 255, 255 ,255 }, false, 1.0f, (uint8)'\000', 5.f);
}

void AMyProject2Ball::Move(const FVector& direction, const FVector& perpendicular, const float Val, const FVector& rawDirection, bool withTraversalHelp, float ReverseVal)
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
		/*if (RaycastWallCheck(direction * Val))
		{*/
		Ball->AddForce(direction * Val * InAirForce * GetWorld()->GetDeltaSeconds(), NAME_None, true);
		//}
	}

	Ball->AddTorque(Torque, NAME_None, true);

	CheckMaxSpeed();
}

void AMyProject2Ball::PrematureSlowTimeStop()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TimeSlowTimerHandle))
	{
		StopSlowTime();
	}
}

bool AMyProject2Ball::CheckIfNearNextFlyLocation()
{
	bool Value = false;
	if (CurrentFlyLocation != FVector::ZeroVector)
	{
		float Dist = FVector::Dist(Ball->GetComponentLocation(), CurrentFlyLocation);
		Value = CurrentFlyLocationDistance < Dist;
		CurrentFlyLocationDistance = Dist;
	}
	return Value;
}

void AMyProject2Ball::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(FVector::DotProduct(HitNormal, FVector::UpVector) >= 0.5f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("YES"));
		bCanJump = true;
	}
	
	if (!bCanRope)
	{
		ReloadRope(DefaultBallVelocity);
	}

	PrematureSlowTimeStop();
}

void AMyProject2Ball::ChangeBalls()
{
	//TODO Implement

}

void AMyProject2Ball::StartSlowTime()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(TimeSlowTimerHandle))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeSlowSlowMultiplier);
		this->CustomTimeDilation = 1.f / TimeSlowSlowMultiplier;
	
		OnGameSpeedChangeEvent.Broadcast(TimeSlowSlowMultiplier);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("BROADCAST"));


		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("START SLOW TIME"));

		GetWorld()->GetTimerManager().SetTimer(TimeSlowTimerHandle, this, &AMyProject2Ball::StopSlowTime, TimeSlowTime * TimeSlowSlowMultiplier);
	}
}

void AMyProject2Ball::StopSlowTime()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	OnGameSpeedChangeEvent.Broadcast(1.f);

	this->CustomTimeDilation = 1.f;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("STOP SLOW TIME"));

	
	GetWorld()->GetTimerManager().ClearTimer(TimeSlowTimerHandle);
}

void AMyProject2Ball::Rope(const FVector & From, const FVector & To, bool LeaveTrace, UPrimitiveComponent* AffectedComponent)
{
	CurrentFlyLocation = To;
	CurrentFlyLocationDistance = TNumericLimits<float>::Max();

	FVector Direction = To - From;
	Direction.Normalize();
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ReloadRope"), DefaultBallVelocity);
	
	//HERE
	MaxBallVelocity = TNumericLimits<float>::Max();
	bCanRope = false;
	bCanJump = false;
	Ball->SetEnableGravity(false);
	AdditionalGravity = FVector::ZeroVector;

	GetWorld()->GetTimerManager().ClearTimer(RopeTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RopeTimerHandle, TimerDelegate, RopeReloadTime, false);

	Ball->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Ball->SetWorldLocation(Ball->GetComponentLocation() + FVector::UpVector*10);
	Ball->AddImpulse(Direction * RopeStrength * Ball->GetMass(), NAME_None, true);
	
	if (LeaveTrace)
	{
		DrawDebugLine(GetWorld(), From, To, FColor{ 255, 255 ,255 }, false, 1.0f, (uint8)'\000', 5.f);
	}

	if (AffectedComponent && AffectedComponent->Mobility == EComponentMobility::Movable && AffectedComponent->IsSimulatingPhysics())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("COMPO MOVE"));
		AffectedComponent->AddForce(-Direction * RopeStrength * Ball->GetMass() * RopeFreeObjectMovementMultplier, NAME_None, true);
	}
	
}

UHUDController * AMyProject2Ball::GetHUDController()
{
	return HUDController;
}

void AMyProject2Ball::ReloadRope(float OldVelocityLimit)
{
	bCanRope = true;
	AdditionalGravity = DefaultAdditionalGravity;
	Ball->SetEnableGravity(true);
	
	GetWorld()->GetTimerManager().ClearTimer(RopeTimerHandle);

	if (OldVelocityLimit <= 0)
	{
		MaxBallVelocity = DefaultBallVelocity;
	}
	else 
	{
		MaxBallVelocity = OldVelocityLimit;
	}

	//if (StopTheBallVelocity)
	//{
		//Ball->SetPhysicsAngularVelocity(FVector::ZeroVector);
		Ball->SetPhysicsLinearVelocity(FVector::ZeroVector);
	//}
}

void AMyProject2Ball::ReloadRope(AGrabableObject * Grabable)
{	
	ReloadRope();
	if (Grabable == ObjectToGrab)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("2"));
		if (Grabable && Grabable->bCanSlowTime)
		{
			StartSlowTime();
		}
		ObjectToGrab->ShowGrabable(false);
		ObjectToGrab = nullptr;

	}
}
