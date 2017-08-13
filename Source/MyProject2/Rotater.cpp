// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Rotater.h"


// Sets default values
ARotater::ARotater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RotationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ROTATION MESH"));	
	RotationMesh->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RotationMesh->SetSimulatePhysics(false);
	RootComponent = RotationMesh;

	RotationSpeed = 10.f;
}

void ARotater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotater::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FRotator rot = RotationMesh->GetComponentRotation();

	rot.Yaw += DeltaTime * RotationSpeed;

	RotationMesh->SetRelativeRotation(rot);
}

