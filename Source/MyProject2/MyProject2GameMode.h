// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
//#include "Blueprint/UserWidget.h"
#include "MyProject2GameMode.generated.h"


UCLASS(minimalapi)
class AMyProject2GameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
public:
	AMyProject2GameMode();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerHUDClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;
};



