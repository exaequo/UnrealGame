// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rotater.generated.h"


UCLASS()
class MYPROJECT2_API ARotater : public AActor /**Actor rotating at tick with given speed*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotater();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* RotationMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerComponent;

	UPROPERTY(EditAnywhere)
		float RotationSpeed;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
