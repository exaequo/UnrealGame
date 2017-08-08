// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SwitchButton.generated.h"

UCLASS()
class MYPROJECT2_API ASwitchButton : public AActor
{
	GENERATED_BODY()
	

public:	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Door;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TriggerMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* TriggerComponent;

	UPROPERTY()
		USceneComponent* SwitchRoot;

	UPROPERTY(VisibleAnywhere)
		bool bTurnedOn;



	UFUNCTION()
		void OnSwitchOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSwitchOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets default values for this actor's properties
	ASwitchButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ManageDoor(bool TurnOn);

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
