// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Cable.h"
#include "GeneratorSwitch.h"
#include "Terminal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CountOfConnectedSwitches: %d"), TerminalRef->CountOfConnectedSwitches));
	}
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
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
