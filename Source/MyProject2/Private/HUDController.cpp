// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/GrabableObject.h"
#include "HUDController.h"


// Sets default values for this component's properties
UHUDController::UHUDController()
{
	PrimaryComponentTick.bCanEverTick = false;
}



FVector2D UHUDController::GetPointerImagePosition() const
{
	return PointerImagePosition;
}

void UHUDController::SetPointerImagePosition(const FVector2D & Value)
{
	PointerImagePosition = Value;
}

bool UHUDController::GetShouldPointerImageRender() const
{
	return bShouldPointerImageRender;
}




FLinearColor UHUDController::GetPointerImageRenderColor() const
{
	FLinearColor ReturnColor = FLinearColor{ 1.f, 1.f, 1.f, 0.f };
	//ReturnColor.A = 0.f;
	if (bShouldPointerImageRender)
	{
		ReturnColor.A = 1.f;
	}
	return ReturnColor;
}

void UHUDController::UpdatePointerImage(const APlayerController* Player, const FVector & WorldLocation)
{
	bShouldPointerImageRender = WorldLocation != AGrabableObject::NothingToGrab;
	
	if (bShouldPointerImageRender)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D ScreenLocation;
		bShouldPointerImageRender = UGameplayStatics::ProjectWorldToScreen(Player, WorldLocation, ScreenLocation);
		
		ScreenLocation *= 1080.f / FMath::Min(ViewportSize.X, ViewportSize.Y);//#MAGIC_UNREAL_NUMBER

		SetPointerImagePosition(ScreenLocation);
	}
}

// Called every frame
void UHUDController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	// ...
}


