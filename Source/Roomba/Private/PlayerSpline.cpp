// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpline.h"

// Sets default values
APlayerSpline::APlayerSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootComponent = SceneComponent;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

