// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/GrabableComponent.h"


// Sets default values for this component's properties
UGrabableComponent::UGrabableComponent()
{
	bWantsBeginPlay = true;
	
	GrabableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrabableMesh"));
	//GrabableMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts
void UGrabableComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGrabableComponent::TurnOn(bool Value)
{

}


//// Called every frame
//void UGrabableComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
//{
//	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
//
//	// ...
//}

