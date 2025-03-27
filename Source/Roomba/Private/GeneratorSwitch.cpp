// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorSwitch.h"

#include "Cable.h"
#include "Generator.h"
#include "ProximityPromptComponent.h"

// Sets default values
AGeneratorSwitch::AGeneratorSwitch()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AGeneratorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpecifiedCable->CableComponent->GetAttachedActor() !=this && RefToGenerator->IsMaster  == false)
	{
		RefToGenerator->SwitchToDisabled();
		if (RefToGenerator->SwitchActor == nullptr || !RefToGenerator->SwitchActor->ActorHasTag("Activated"))
		{
			RefToGenerator->SwitchLightOff();
		}
	}

	if (SpecifiedCable->CableComponent->GetAttachedActor() == this && MasterGenerator->IsConnectedToRope == true )
	{
		RefToGenerator->SwitchToEnabled();
		if (RefToGenerator->SwitchActor == nullptr || RefToGenerator->SwitchActor->ActorHasTag("Activated"))
		{
			RefToGenerator->SwitchLightOn();
		}
	}

	IsConnectedToRope = RefToGenerator->IsConnectedToRope;
	
}


void AGeneratorSwitch::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == SpecifiedCable->CableComponent->GetAttachedActor())
	{
		SpecifiedCable->CableComponent->SetAttachEndTo(this,FName("SwitchMesh"));

		if (SpecifiedCable->CableComponent->GetAttachedActor() == this && IsMasterSwitch)
		{
			RefToGenerator->SwitchToEnabled();
		}
	}
}
