// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorSwitch.generated.h"

class ACable;
class UProximityPromptComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ROOMBA_API AGeneratorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneratorSwitch();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SwitchMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	
	UPROPERTY(EditAnywhere)
	UMaterial*  EnabledMaterial;
	UPROPERTY(EditAnywhere)
	UMaterial*  DisabledMaterial;

	UPROPERTY(EditAnywhere)
	ACable* SpecifiedCable;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool IsConnectedToRope = false;

	void SwitchToEnabled();
	void SwitchToDisabled();
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
