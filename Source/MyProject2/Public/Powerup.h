// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Powerup.generated.h"

UCLASS()
class MYPROJECT2_API APowerup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerup();

	UPROPERTY()
		USceneComponent* PowerupRoot;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TriggerMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerComponent;

	UPROPERTY(EditAnywhere)
		bool countsAsCollectible;

	UPROPERTY(EditAnywhere)
		int powerGranting;

	UFUNCTION()
		void OnPowerupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	
	
};
