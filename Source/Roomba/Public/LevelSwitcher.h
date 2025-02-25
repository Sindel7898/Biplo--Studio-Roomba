// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSwitcher.generated.h"

class UBoxComponent;
class ARoombaMovement;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewLevelReached, FName, LevelName);

UCLASS()
class ROOMBA_API ALevelSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSwitcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerCharacter;

	UPROPERTY()
	ARoombaMovement* RoombaCharacter;
	
	UPROPERTY(EditAnywhere, Category = LevelData)
	FName LevelToLoad;

	// Broadcast when the next level is reached
	UPROPERTY(BlueprintAssignable, Category="Events")
	FNewLevelReached NewLevelReached;
	
	void LoadNextLevel();
	
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
