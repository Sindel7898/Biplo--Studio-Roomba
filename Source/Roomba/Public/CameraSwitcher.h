// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSwitcher.generated.h"

class ARoombaCharacter;
class UBoxComponent;
UCLASS()
class ROOMBA_API ACameraSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSwitcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	
	UPROPERTY()
	ARoombaCharacter* RoombaCharacter;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerCharacter;
	
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float InterpolationRate;
	UPROPERTY(EditAnywhere)
	FVector TargetCameraPosition;
	UPROPERTY(EditAnywhere)
	FRotator TargetCameraRotation;

	UPROPERTY(EditAnywhere)
	FVector PlayerForwardDirection;
	UPROPERTY(EditAnywhere)
	FVector PlayerRightDirection;

	
};
