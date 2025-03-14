// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Cable.h"
#include "GeneratorSwitch.h"
#include "Components/BoxComponent.h"

// Sets default values
AGenerator::AGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
	
	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	GeneratorMesh->SetupAttachment(RootComponent);
}

void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (MasterGenerator)
	{
		if (MasterGenerator->IsConnectedToRope == true && this->IsConnectedToRope == true )
		{
			SwitchToEnabled();
		}
	}*/
	
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();

	/*if (IsMaster)
	{
		SwitchToEnabled();
	}*/
}

void AGenerator::SwitchToEnabled()
{
	IsConnectedToRope = true;
	GeneratorMesh->SetMaterial(0,EnabledMaterial);
}

void AGenerator::SwitchToDisabled()
{
	IsConnectedToRope = false;
	GeneratorMesh->SetMaterial(0,DisabledMaterial);
}
