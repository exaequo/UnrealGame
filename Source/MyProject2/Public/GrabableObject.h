// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrabableObject.generated.h"

UCLASS()
class MYPROJECT2_API AGrabableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabableObject();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GrabableMesh;
	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerShape;
	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, Category = Materials)
		UMaterialInterface* SelectedMaterial;
	UPROPERTY(EditAnywhere, Category = Materials)
		UMaterialInterface* NotSelectedMaterial;
	UPROPERTY(EditAnywhere)
		bool bAbsoluteGrab;


	UFUNCTION()
		void OnBallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ShowGrabable(bool Value);

	FVector GetGrabablePosition() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//
	//// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	
	
};