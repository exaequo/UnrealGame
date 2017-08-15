// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HUDController.generated.h"

/** Used to control HUD of the player */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT2_API UHUDController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHUDController();

	/** Returns position of the pointer image */
	UFUNCTION(BlueprintPure, Category = "HUD")
		FVector2D GetPointerImagePosition() const;

	/** Sets desired image position to given vector */
	UFUNCTION(BlueprintPure, Category = "HUD")
		void SetPointerImagePosition(const FVector2D& Value);

	/** Defines wheter or not the Pointer Image should render*/
	UFUNCTION(BlueprintPure, Category = "HUD")
		bool GetShouldPointerImageRender() const;

	/** Returns color the pointer image should use */
	UFUNCTION(BlueprintPure, Category = "HUD")
		FLinearColor GetPointerImageRenderColor() const;

	/** Updates the Pointer Image by translating world point to screen point */
	void UpdatePointerImage(const APlayerController* Player, const FVector& WorldLocation);

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
private:

	/** Current position of the pointer Image to be seen on the screen */
	FVector2D PointerImagePosition;
	
	/** Value indicating wheter or not PointerImage on the HUD should be rendered */
	bool bShouldPointerImageRender;

	
};
