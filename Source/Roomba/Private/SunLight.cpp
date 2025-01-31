// Fill out your copyright notice in the Description page of Project Settings.


#include "SunLight.h"

#include "Components/DirectionalLightComponent.h"

// Sets default values
ASunLight::ASunLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	DirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun Light"));
	DirectionalLightComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASunLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SunState == SunLightTransionStates::TOSPECIFICPOSITION)
	{
		// interpolation to next sun rotation 
		FRotator NewRotation = FMath::RInterpTo(DirectionalLightComponent->GetComponentRotation(), TargetPosition, DeltaTime, InterpolationSpeed);
		DirectionalLightComponent->SetRelativeRotation(NewRotation);
	}
	
}

