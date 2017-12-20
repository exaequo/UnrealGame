// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class MYPROJECT2_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TriggerMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerComponent;

	//UPROPERTY(EditAnywhere)
	//	class UGrabableComponent* GrabableComponent;

	UPROPERTY()
		USceneComponent* SwitchRoot;

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerShape;

	UPROPERTY(EditAnywhere)
		int checkpointNumber;

	static int highestCheckpoint;
	static ACheckpoint* currentCheckpoint;


	void OnSwitchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sets default values for this actor's properties
	ACheckpoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	static void TeleportPlayerToLastCheckpoint(class AMyProject2Ball* Ball);
	
	bool active;
};
