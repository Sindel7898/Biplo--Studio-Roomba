// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "Cable.generated.h"

class AGeneratorSwitch;
class UBoxComponent;

UCLASS()
class ROOMBA_API ACable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACable();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCableComponent* CableComponent;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	
	/*
	virtual bool IsAttachedTo(const AActor* Other) const override;
	*/
	
	/*UPROPERTY(EditAnywhere)
    AActor* GeneratorSwitch1;

	UPROPERTY(EditAnywhere)
	AActor* GeneratorSwitch2;

	UPROPERTY(EditAnywhere)
	AActor* GeneratorSwitch3;

	UPROPERTY(EditAnywhere)
	AActor* GeneratorSwitch4;*/

	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere)
	bool IsCableOnFloor = false;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
