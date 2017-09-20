// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "Public/GrabableObject.h"
#include "Public/HUDController.h"
#include "MyProject2Ball.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSpeedChange, float, NewSpeed);


UCLASS(config=Game)
class AMyProject2Ball : public APawn /**Main Pawn controllable by player*/
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SecondBall;

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

	/** Root component of the Pawn */
	UPROPERTY(EditAnywhere, Category = Ball)
	USceneComponent* RootComp;

	/** Triggering part of the ball */
	UPROPERTY(EditAnywhere)
	UShapeComponent* TriggerComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameSpeedChange OnGameSpeedChangeEvent;

	/** Defines if the player can use rope */
	bool bCanRope;

	/** Defines wheter or not player is on the door switch (OBSOLETE) */
	bool bIsOnSwitch;

	/** Indicates whether we can currently jump */
	bool bCanJump;

	/** Return a pointer to the current HUD Controller*/
	UFUNCTION(BlueprintPure, Category = "HUD")
		UHUDController* GetHUDController();

	/** Reloads the rope and restores the gravity to the Ball */
	UFUNCTION()
	void ReloadRope(float OldVelocityLimit = -1.f);

	/** Reloads the rope and restores the gravity to the Ball (Version to be used by GrabableObjects) */
	void ReloadRope(AGrabableObject* Grabable);

	virtual void Tick(float DeltaSeconds) override;

protected:
	/** Pointer to the object that the ball will rope to if player presses the Rope button */
	UPROPERTY()
		AGrabableObject* ObjectToGrab;

	UPROPERTY(EditAnywhere, Category = "HUD")
		UHUDController* HUDController;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category = Ball)
		float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category = Ball)
		float RollTorque;

	/** Force that will be applied when players try to move mid air */
	UPROPERTY(EditAnywhere, Category = Ball)
		float InAirForce;

	/** Maximal linear velocity the ball can achieve */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MaxBallVelocity;

	/** Maximal angular velocity the ball can achieve */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MaxBallAngularVelocity;

	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeUpOffset;

	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeAbsoluteErrorValue;

	/** Values smaller than this will be blindspotted */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeXYBlindSpotDistance;

	/** Values above this will be blindspotted */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeZBlindSpotDistance;

	/** Amount of force that will be applied to ball when roping */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeStrength;

	/** Maximal distance the ball can rope to */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeLength;

	/** Time after rope will reload regardless of hitting floor */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeReloadTime;

	/** Percentage of force that will be applied to movable objects when the rope hits them */
	UPROPERTY(EditAnywhere, Category = Rope)
		float RopeFreeObjectMovementMultplier;

	/** Amount of constant force that is given to the Pawn at Tick */
	UPROPERTY(EditAnywhere, Category = Ball)
		FVector AdditionalGravity;

	/** Sensitivity of the camera during mouse movements */
	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraRotationSensitivity;

	/** Affects how high the raycasts for finding grabable objects for rope will go (affects the angle that the raycasts are shot at)*/
	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraRaycastUpModifier;

	FVector LocationToGrab;

	FVector CurrentFlyLocation;
	float CurrentFlyLocationDistance;

	/** Handler for Timer of the rope */
	FTimerHandle RopeTimerHandle;

	/** Handler for Timer of the slow time */
	FTimerHandle TimeSlowTimerHandle;

	/** Default velocity of the ball (Set at the BeginPlay event) */
	float DefaultBallVelocity;	

	/** Default Gravity of the ball (Set at BeginPlay event) */
	FVector DefaultAdditionalGravity;

	/** Time for which the time stop effect will take place (time between call of Start and Stop SlowTime functions) */
	static float TimeSlowTime;

	/** Multiplier in which the time will be scaled by during time slow */
	static float TimeSlowSlowMultiplier;

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	FVector GetPositionFromRaycast(const FHitResult& HitResult) const;
	///** Called for side to side input */
	//void MoveRight(float Val);

	///** Called to move ball forwards and backwards */
	//void MoveForward(float Val);

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Sets the Yaw of the camera */
	UFUNCTION()
	void RotateCamera(float Val);

	/** Sets the Pitch of the camera */
	UFUNCTION()
	void PitchCamera(float Val);

	/** Used in order to possess second ball (OBSOLETE) */
	UFUNCTION()
	void CheckForPossession();

	/** Will be used for creating a ball clone */
	UFUNCTION()
	void ChangeBalls();

	/** Starts the slow of everything but the ball */
	UFUNCTION()
	void StartSlowTime();

	/** Stops the slow of everything but the ball */
	UFUNCTION()
	void StopSlowTime();

	/** Turns of gravity and then adds impulse to the ball making it move in a specified direction */
	void Rope(const FVector& From, const FVector& To, bool LeaveTrace = true, UPrimitiveComponent* AffectedComponent = nullptr);
	
	/** Checks if ball should be able to use the forces that are added in the air */
	bool RaycastWallCheck(const FVector& direction) const;

	/** Checks if the linear and angular speeds are BE the Max amounts. If not, then they are set to the max */
	void CheckMaxSpeed();

	/** Sets the rotation of helper arrows */
	void SetArrowRotation();

	/** Continous raycast that is being cast at Tick to check for the rope-grabable objects */
	void CameraPointRaycast();

	/** Draws a line between given points */
	void DrawPossessionLine(const FVector& firstPoint, const FVector& secondPoint) const;

	/** Default Move function used in MoveForward and MoveRight */
	void Move(const FVector& direction, const FVector& perpendicular, const float Val, const FVector& rawDirection, bool withTraversalHelp = false, float ReverseVal = 1.f);

	void PrematureSlowTimeStop();

	bool CheckIfNearNextFlyLocation();

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	friend class ASlowTimeArea;
};
