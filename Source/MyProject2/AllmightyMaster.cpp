// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/Powerup.h"
#include "AllmightyMaster.h"
//
//AAllmightyMaster* AAllmightyMaster::instance;
TArray<APowerup*> AAllmightyMaster::AllPowerups;
TArray<AMyProject2Ball*> AAllmightyMaster::AllBalls;
int32 AAllmightyMaster::PowerupCount = 0;
const FVector AAllmightyMaster::BoundsPointMapping[8] = {
	FVector(1, 1, 1),
	FVector(1, 1, -1),
	FVector(1, -1, 1),
	FVector(1, -1, -1),
	FVector(-1, 1, 1),
	FVector(-1, 1, -1),
	FVector(-1, -1, 1),
	FVector(-1, -1, -1)
};


AAllmightyMaster::AAllmightyMaster()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AllPowerups.Empty();
	AllBalls.Empty();
	PowerupCount = 0;
}

void AAllmightyMaster::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("GAME START"));
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, TEXT("PRESS 'WSAD' to move, Space/LMB to jump, RMB to rope"));
}

void AAllmightyMaster::PowerupCollected(APowerup* Collected)
{
	if (AllPowerups.Contains(Collected))
	{
		AllPowerups.Remove(Collected);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Collected %i/%i"), PowerupCount - AllPowerups.Num(), PowerupCount));

	if (AllPowerups.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("WON \nTime spent %f"), Collected->GetWorld()->GetTimeSeconds()));
		RestartLevel(Collected->GetWorld()->GetFirstPlayerController());
	}
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

void AAllmightyMaster::RestartLevel(APlayerController* Controller)
{
	Controller->ConsoleCommand(TEXT("RestartLevel"));
}


