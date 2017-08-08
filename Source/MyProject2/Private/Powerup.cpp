// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "MyProject2Ball.h"
#include "AllmightyMaster.h"
#include "Public/Powerup.h"


// Sets default values
APowerup::APowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

	PowerupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = PowerupRoot;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->AttachToComponent(PowerupRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerComponent->AttachToComponent(TriggerMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerup::OnPowerupOverlap);
}

void APowerup::OnPowerupOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);

	UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	if (Shape != nullptr && Ball != nullptr)
	{
		AAllmightyMaster::PowerupCollected(this);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("ShouldCollect"));
		Destroy();
	}
}

// Called when the game starts or when spawned
void APowerup::BeginPlay()
{
	Super::BeginPlay();
	
	AAllmightyMaster::AddPowerup(this);
}

// Called every frame
//void APowerup::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}

