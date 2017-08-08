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

	//RotationTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ROTATION Trigger"));
	////RotationTrigger->BodyInstance.SetCollisionEnabled(UCollisionProfile::);
	//RotationTrigger->SetSimulatePhysics(false);
	////RotationTrigger->OnComponentBeginOverlap.AddUnique(this, &ARotater::OnTriggerOverlapBegin);
	////RotationTrigger->OnComponentEndOverlap.AddUnique(this, &);

	//RotationTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARotater::OnTriggerOverlapBegin);
	//RotationTrigger->OnComponentEndOverlap.AddDynamic(this, );

	//TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	//TriggerComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ARotater::OnTriggerOverlapBegin);
	//TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ARotater::OnTriggerOverlapEnd);
	

	RotationSpeed = 10.f;
}

void ARotater::OnTriggerOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void ARotater::OnTriggerOverlapEnd(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
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

