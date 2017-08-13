// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "MyProject2Ball.h"
#include "AllmightyMaster.h"
#include "Public/Powerup.h"



APowerup::APowerup()
{
	PowerupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = PowerupRoot;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->AttachToComponent(PowerupRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerComponent->AttachToComponent(TriggerMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerup::OnPowerupOverlap);
}

/**Checks if the overlap was conducted by player. If so, the powerup is destroyed. */
void APowerup::OnPowerupOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);

	UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	if (Shape != nullptr && Ball != nullptr)
	{
		AAllmightyMaster::PowerupCollected(this);
		
		Destroy();
	}
}

void APowerup::BeginPlay()
{
	Super::BeginPlay();
	
	AAllmightyMaster::AddPowerup(this);
}
