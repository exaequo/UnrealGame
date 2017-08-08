// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "MyProject2Ball.generated.h"

UCLASS(config=Game)
class AMyProject2Ball : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Arrow;

public:
	AMyProject2Ball();

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category=Ball)
	float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category=Ball)
	float RollTorque;

	UPROPERTY(EditAnywhere)
	UShapeComponent* TriggerComponent;

	UPROPERTY(EditAnywhere, Category = Ball)
	float InAirForce;

	UPROPERTY(EditAnywhere, Category = Ball)
	float MaxBallVelocity;
	
	UPROPERTY(EditAnywhere, Category = Ball)
	float MaxBallAngularVelocity;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraRotationSensitivity;


	bool bIsOnSwitch;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	bool bCanJump;

protected:
	//UPROPERTY(VisibleAnywhere)
	//	AMyProject2Ball* BallInside;

	UPROPERTY(EditAnywhere)
		float FTriggerSphereSize;

	
	

	void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action. */
	void Jump();

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RotateCamera(float Val);

	UFUNCTION()
	void PitchCamera(float Val);

	UFUNCTION()
	void CheckForPossession();

	bool RaycastWallCheck(const FVector& direction) const;

	void CheckMaxSpeed();

	void SetArrowRotation();
	// End of APawn interface

protected:
	void DrawPossessionLine(const FVector firstPoint, const FVector secondPoint) const;

	void Move(const FVector direction, const FVector perpendicular, const float Val, const FVector rawDirection, bool withTraversalHelp = false, float ReverseVal = 1.f);

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
