// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Powerup.h"
#include "MyProject2Ball.h"
#include "AllmightyMaster.generated.h"

UCLASS()
class MYPROJECT2_API AAllmightyMaster : public AActor
{
	GENERATED_BODY()
private:
	static TArray<APowerup*> AllPowerups;
	static TArray<AMyProject2Ball*> AllBalls;
public:	
	
	AAllmightyMaster();
	
	static int32 PowerupCount;
	
	static const FVector BoundsPointMapping[8];

	virtual void BeginPlay() override;

	static void PowerupCollected(APowerup* Collected);
	static void AddPowerup(APowerup* Powerup);
	
	static void AddBall(AMyProject2Ball* Ball);
	static AMyProject2Ball* GetNextBall(AMyProject2Ball* Ball);

	static void RestartLevel(APlayerController* Controller);

};
