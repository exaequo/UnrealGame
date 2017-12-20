// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Checkpoint.h"
#include "MyProject2Ball.h"

int ACheckpoint::highestCheckpoint = -1;
ACheckpoint* ACheckpoint::currentCheckpoint = nullptr;

// Sets default values
ACheckpoint::ACheckpoint()
{
	SwitchRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SwitchRoot;


	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->AttachToComponent(SwitchRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerComponent->AttachToComponent(SwitchRoot, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSwitchOverlap);
	
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	active = false;
}


//void ACheckpoint::OnBallOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("fgsadjkgoia/%i"), checkpointNumber));
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("-1"));
//	if (Ball)
//	{
//		if (highestCheckpoint < checkpointNumber)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Checkpoint added /%i"), checkpointNumber));
//			highestCheckpoint = checkpointNumber;
//			currentCheckpoint = this;
//		}
//	}
//}

void ACheckpoint::OnSwitchOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("fgsadjkgoia/%i"), checkpointNumber));
	active = true;
	/*UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	if (Shape != nullptr && Ball != nullptr)
	{
		ManageDoor(true);

		Ball->bIsOnSwitch = true;
	}*/

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("fgsadjkgoia/%i"), checkpointNumber));
	if (active)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("1"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("0"));

	}
}

void ACheckpoint::TeleportPlayerToLastCheckpoint(AMyProject2Ball * Ball)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Want to transfer to /%i"), highestCheckpoint));
	if (currentCheckpoint)
	{
		
	}
}

