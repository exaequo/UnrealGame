// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "Public/GrabableComponent.h"
#include "MyProject2Ball.h"
#include "AllmightyMaster.h"
#include "Public/SwitchButton.h"

int ASwitchButton::highestCheckpointNumber = -1; \
ASwitchButton* ASwitchButton::currentCheckpoint = nullptr;


// Sets default values
ASwitchButton::ASwitchButton()
{
	SwitchRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SwitchRoot;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->AttachToComponent(SwitchRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->AttachToComponent(SwitchRoot, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerComponent->AttachToComponent(TriggerMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ASwitchButton::OnSwitchOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ASwitchButton::OnSwitchOverlapEnd);
	
	//GrabableComponent = CreateDefaultSubobject<UGrabableComponent>(TEXT("Grabable"));
	//GrabableComponent->AttachToComponent(Door, FAttachmentTransformRules::KeepRelativeTransform);
	
	bTurnedOn = false;
	PrimaryActorTick.bCanEverTick = true;
	bReverseSwitch = false;
	checkpointNumber = 0;
	bIsEnd = false;
}

// Called when the game starts or when spawned
void ASwitchButton::BeginPlay()
{
	Super::BeginPlay();
	highestCheckpointNumber = -1;
	//ManageDoor(false);
	
}

void ASwitchButton::ManageDoor(bool TurnOn)
{
	if (bReverseSwitch)
	{
		TurnOn = !TurnOn;
	}

	bTurnedOn = TurnOn;
	

	Door->SetVisibility(!TurnOn);
	
	if (TurnOn)
	{
		Door->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Door->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ASwitchButton::OnSwitchOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);
	
	UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	if (Shape && Ball)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("fgsadjkgoia/%i"), checkpointNumber));
		//ManageDoor(true);

		//Ball->bIsOnSwitch = true;

		if (highestCheckpointNumber < checkpointNumber)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Checkpoint added /%i"), checkpointNumber));
			highestCheckpointNumber = checkpointNumber;
			currentCheckpoint = this;
			if (bIsEnd)
			{
				AAllmightyMaster::RestartLevel(GetWorld()->GetFirstPlayerController());
			}
		}
	}

}

void ASwitchButton::OnSwitchOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);

	//UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	//if (Shape != nullptr && Ball != nullptr)
	//{
	//	/*ManageDoor(false);
	//	Ball->bIsOnSwitch = false;*/
	//}
}

//// Called every frame
void ASwitchButton::Tick( float DeltaTime )
{
	//Super::Tick(DeltaTime);

	//FColor color{ 255, 0, 0 };
	//
	//if (bTurnedOn ^ bReverseSwitch)
	//{
	//	color = FColor{ 0, 255, 0 };
	//}

	////FString str = FString::Printf(TEXT("Character Position is %s"), Door->GetComponentLocation().ToCompactString());
	////GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, str);
	//DrawDebugLine(GetWorld(), Door->GetComponentLocation(), TriggerMesh->GetComponentLocation(), color, false,-1.f, (uint8)'\000', 5.f);
}

void ASwitchButton::TeleportPlayerToLastCheckpoint(AMyProject2Ball * Ball)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Want to transfer to /%i"), highestCheckpointNumber));
	if (currentCheckpoint)
	{
		if (Ball)
		{
			Ball->GetBall()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
			Ball->GetBall()->SetAllPhysicsAngularVelocity(FVector::ZeroVector);
			Ball->GetBall()->SetWorldLocation(currentCheckpoint->TriggerMesh->GetComponentLocation());
		}
	}
}

