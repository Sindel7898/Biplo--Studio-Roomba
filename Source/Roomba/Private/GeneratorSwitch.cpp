// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorSwitch.h"

#include "Cable.h"
#include "ProximityPromptComponent.h"

// Sets default values
AGeneratorSwitch::AGeneratorSwitch()
{
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(SwitchMesh);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&AGeneratorSwitch::OverlapBegins);

}

// Called when the game starts or when spawned
void AGeneratorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGeneratorSwitch::SwitchToEnabled()
{
	IsConnectedToRope = true;
	SwitchMesh->SetMaterial(0,EnabledMaterial);

}

void AGeneratorSwitch::SwitchToDisabled()
{
	IsConnectedToRope = false;
	SwitchMesh->SetMaterial(0,DisabledMaterial);
}

void AGeneratorSwitch::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == SpecifiedCable->CableComponent->GetAttachedActor())
	{
		SpecifiedCable->CableComponent->SetAttachEndTo(this,FName("SwitchMesh"));
		SwitchToEnabled();
	}
	
	
	
}
