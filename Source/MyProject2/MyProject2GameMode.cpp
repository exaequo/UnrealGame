// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MyProject2.h"
#include "MyProject2GameMode.h"
#include "MyProject2Ball.h"
#include "Blueprint/UserWidget.h"

void AMyProject2GameMode::BeginPlay()
{
	Super::BeginPlay();

	//AMyProject2Ball* Ball = Cast<AMyProject2Ball>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (PlayerHUDClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		

		if (CurrentWidget != nullptr)
		{	
			CurrentWidget->AddToViewport();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("WIDGET NULL"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("HUD NULL"));
	}
}

AMyProject2GameMode::AMyProject2GameMode()
{
	// set default pawn class to our ball
	//DefaultPawnClass = AMyProject2Ball::StaticClass();
}
