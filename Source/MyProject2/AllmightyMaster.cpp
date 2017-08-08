// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "AllmightyMaster.h"


// Sets default values
AAllmightyMaster::AAllmightyMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllmightyMaster::BeginPlay()
{
	
	Super::BeginPlay();
	if (StartPawn != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("WASNT A NULL"));
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		controller->UnPossess();
		controller->Possess(StartPawn);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("WAS A NULL"));
	}
	

}

// Called every frame
void AAllmightyMaster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

