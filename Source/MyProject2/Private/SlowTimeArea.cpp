// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "SlowTimeArea.h"
#include "MyProject2Ball.h"


// Sets default values
ASlowTimeArea::ASlowTimeArea()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	GrabableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grabable Mesh"));
	GrabableMesh->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerShape = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerShape->AttachToComponent(GrabableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerShape->OnComponentBeginOverlap.AddDynamic(this, &ASlowTimeArea::OnBallOverlap);
}


void ASlowTimeArea::OnBallOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("SHOUUUULD"));
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);

	UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("-1"));
	if (Ball)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("SLOOOOW"));
		Ball->StartSlowTime();
	}
}