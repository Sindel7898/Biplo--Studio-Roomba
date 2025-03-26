// Fill out your copyright notice in the Description page of Project Settings.


#include "Terminal.h"
#include "GeneratorSwitch.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATerminal::ATerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	GeneratorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GeneratorSwitch1->IsConnectedToRope && GeneratorSwitch2->IsConnectedToRope && GeneratorSwitch3->IsConnectedToRope && GeneratorSwitch4->IsConnectedToRope)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "End");
	}
}

