// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/GrabableObject.h"
#include "MyProject2Ball.h"
#include "HUDController.h"


// Sets default values for this component's properties
UHUDController::UHUDController()
{
	PrimaryComponentTick.bCanEverTick = false;

	OverlayImageAlpha = 0.f;

	PointerImageScale = 1.f;
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

FVector2D UHUDController::GetPointerImageScale() const
{
	return FVector2D{ PointerImageScale, PointerImageScale };
}

float UHUDController::GetOverlayImageColorAlpha() const
{
	return OverlayImageAlpha;
}

void UHUDController::SetOverlayImageAlpha(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("BROADCAST"));
	Value = FMath::Clamp(Value, 0.f, 1.f);
	OverlayImageAlpha = (1.f - Value) / 2.f;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Overlay A: %f"), OverlayImageAlpha));
}

void UHUDController::Init(AMyProject2Ball * Parent) const
{

	Parent->OnGameSpeedChangeEvent.AddDynamic(this, &UHUDController::SetOverlayImageAlpha);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("INIT"));
}

void UHUDController::UpdatePointerImage(const APlayerController* Player, const FVector & WorldLocation, float ScaleFactor)
{
	bShouldPointerImageRender = WorldLocation != AGrabableObject::NothingToGrab;
	
	if (bShouldPointerImageRender)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D ScreenLocation;
		bShouldPointerImageRender = UGameplayStatics::ProjectWorldToScreen(Player, WorldLocation, ScreenLocation);
		
		ScreenLocation *= 1080.f / FMath::Min(ViewportSize.X, ViewportSize.Y);//#MAGIC_UNREAL_NUMBER

		SetPointerImagePosition(ScreenLocation);

		PointerImageScale = FMath::Clamp(ScaleFactor, 0.2f, 1.f);
	}
}

// Called every frame
void UHUDController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	// ...
}


