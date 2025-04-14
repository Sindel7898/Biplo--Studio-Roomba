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
	
	APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	cameramanager->StartCameraFade(1, 0, 2.5, FLinearColor::Black, false, true);
}


// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GeneratorSwitch1->IsConnectedToRope && GeneratorSwitch2->IsConnectedToRope && GeneratorSwitch3->IsConnectedToRope && GeneratorSwitch4->IsConnectedToRope)
	{
		APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		cameramanager->StartCameraFade(0, 1, 1.5, FLinearColor::Black, false, true);

		FTimerHandle FadeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &ATerminal::OnFadeComplete, 1.5f, false);
	}
}

void ATerminal::OnFadeComplete()
{
	UGameplayStatics::OpenLevel(GetWorld(), "End");
}