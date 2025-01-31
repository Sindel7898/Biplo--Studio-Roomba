// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightTrigger.generated.h"

class UBoxComponent;
class ASunLight;

UCLASS()
class ROOMBA_API ALightTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> SunLightClass;

	UPROPERTY()
	ASunLight* SunLightRef;
	
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpulation)
	float InterpolationRate= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpulation)
	FRotator TargetSunPosition;
};
