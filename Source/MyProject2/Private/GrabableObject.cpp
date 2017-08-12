// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"
#include "MyProject2Ball.h"
#include "GrabableObject.h"


// Sets default values
AGrabableObject::AGrabableObject()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	TriggerShape = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerShape->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	GrabableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grabable Mesh"));
	GrabableMesh->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerShape->OnComponentBeginOverlap.AddDynamic(this, &AGrabableObject::OnBallOverlap);

	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Selected(TEXT("Material'/Game/MINE/Materials/GrabbedMaterial.GrabbedMaterial'"));
	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_NotSelected(TEXT("Material'/Game/MINE/Materials/NotGrabbedMaterial.NotGrabbedMaterial'"));

	SelectedMaterial = Material_Selected.Object;
	NotSelectedMaterial = Material_NotSelected.Object;
	
	GrabableMesh->SetMaterial(0, NotSelectedMaterial);
}

void AGrabableObject::OnBallOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMyProject2Ball* Ball = dynamic_cast<AMyProject2Ball*>(OtherActor);

	UStaticMeshComponent* Shape = dynamic_cast<UStaticMeshComponent*>(OtherComp);

	if (Shape != nullptr && Ball != nullptr)
	{
		Ball->ReloadRope();
	}
}

void AGrabableObject::ShowGrabable(bool Value)
{
	UMaterialInterface* mat = NotSelectedMaterial;

	if (Value)
	{
		mat = SelectedMaterial;
	}

	GrabableMesh->SetMaterial(0, mat);
}

FVector AGrabableObject::GetGrabablePosition() const
{
	//if (!bAbsoluteGrab)
	//{
	//	FVector Loc = RayHitPosition;
	//	Loc.Z = GrabableMesh->GetComponentLocation().Z;
	//	return Loc;
	//}

	return GrabableMesh->GetComponentLocation();
}

void AGrabableObject::BeginPlay()
{
	GrabableMesh->SetVisibility(false);
}
