// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SlowTimeArea.generated.h"

UCLASS()
class MYPROJECT2_API ASlowTimeArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowTimeArea();

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerShape;
	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GrabableMesh;

	UFUNCTION()
		void OnBallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
