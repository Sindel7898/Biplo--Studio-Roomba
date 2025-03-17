// Fill out your copyright notice in the Description page of Project Settings.


#include "Crane.h"

// Sets default values
ACrane::ACrane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	CraneRotator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneRotatorMesh"));
	CraneRotator->SetupAttachment(RootComponent);

	CraneGrabber = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneGrabberMesh"));
	CraneGrabber->SetupAttachment(CraneRotator);
}

// Called when the game starts or when spawned
void ACrane::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACrane::RotateCraneLeft(float RotationAmount)
{
	FRotator NewRotation = CraneRotator->GetRelativeRotation();
	NewRotation.Yaw = FMath::FInterpTo(NewRotation.Yaw , NewRotation.Yaw + RotationAmount, GetWorld()->GetDeltaSeconds(), RotationalSpeed);
	CraneRotator->SetRelativeRotation(NewRotation);
}

void ACrane::RotateCraneRight(float RotationAmount)
{
	FRotator NewRotation = CraneRotator->GetRelativeRotation();
	NewRotation.Yaw = FMath::FInterpTo(NewRotation.Yaw , NewRotation.Yaw - RotationAmount, GetWorld()->GetDeltaSeconds(), RotationalSpeed);
	CraneRotator->SetRelativeRotation(NewRotation);

}

void ACrane::MoveCraneUp(float MoveAmount)
{
	FVector NewLocation = 	CraneGrabber->GetRelativeLocation();
	NewLocation.X = FMath::FInterpTo(NewLocation.X , NewLocation.X +  MoveAmount, GetWorld()->GetDeltaSeconds(), HorizontalSpeed);
	NewLocation.X  = FMath::Clamp(NewLocation.X,-460.0f,-60.0f);
	
	CraneGrabber->SetRelativeLocation(NewLocation);
}


void ACrane::MoveCraneDown(float MoveAmount)
{
	FVector NewLocation = 	CraneGrabber->GetRelativeLocation();
	NewLocation.X = FMath::FInterpTo(NewLocation.X , NewLocation.X  -   MoveAmount, GetWorld()->GetDeltaSeconds(), HorizontalSpeed);
	NewLocation.X  = FMath::Clamp(NewLocation.X,-460.0f,-60.0f);
	
	CraneGrabber->SetRelativeLocation(NewLocation);
}