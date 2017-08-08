// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AllmightyMaster.generated.h"

UCLASS()
class MYPROJECT2_API AAllmightyMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
		APawn* StartPawn;

	//static AAllmightyMaster* MasterInstance;
	// Sets default values for this actor's properties
	AAllmightyMaster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
