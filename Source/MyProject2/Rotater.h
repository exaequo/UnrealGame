// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rotater.generated.h"

UCLASS()
class MYPROJECT2_API ARotater : public AActor
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

	UFUNCTION()
		void OnTriggerOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};