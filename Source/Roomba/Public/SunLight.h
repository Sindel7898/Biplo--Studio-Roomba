// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SunLight.generated.h"


UENUM()
enum SunLightTransionStates
{
	TOSPECIFICPOSITION UMETA(DisplayName = "TOSPECIFICPOSITION"),
	Static UMETA(DisplayName = "TOSPECIFICPOSITION"),

};
class UDirectionalLightComponent;
UCLASS()
class ROOMBA_API ASunLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASunLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UDirectionalLightComponent* DirectionalLightComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	SunLightTransionStates SunState = SunLightTransionStates::Static;
	
	float InterpolationSpeed = 1.0f;
	FRotator TargetPosition;
};
