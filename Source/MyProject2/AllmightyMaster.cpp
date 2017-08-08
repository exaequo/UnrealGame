// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/Powerup.h"
#include "AllmightyMaster.h"
//
//AAllmightyMaster* AAllmightyMaster::instance;
TArray<APowerup*> AAllmightyMaster::AllPowerups;
TArray<AMyProject2Ball*> AAllmightyMaster::AllBalls;
int32 AAllmightyMaster::PowerupCount = 0;

// Sets default values
AAllmightyMaster::AAllmightyMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AllPowerups.Empty();
	AllBalls.Empty();
	PowerupCount = 0;
}

// Called when the game starts or when spawned
void AAllmightyMaster::BeginPlay()
{

	//MasterInstance = this;
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

void AAllmightyMaster::PowerupCollected(APowerup* Collected)
{
	if (AllPowerups.Contains(Collected))
	{
		AllPowerups.Remove(Collected);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Collected %i/%i"), PowerupCount - AllPowerups.Num(), PowerupCount));
}

void AAllmightyMaster::AddPowerup(APowerup* Powerup)
{
	AAllmightyMaster::AllPowerups.AddUnique(Powerup);
	++PowerupCount;
}

void AAllmightyMaster::AddBall(AMyProject2Ball* Ball)
{
	AllBalls.Add(Ball);
}

AMyProject2Ball* AAllmightyMaster::GetNextBall(AMyProject2Ball* Ball)
{
	int32 index;
	if (AllBalls.Find(Ball, index))
	{
		return AllBalls[(++index) % AllBalls.Num()];
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("BallChange"));
	}

	return nullptr;
}


