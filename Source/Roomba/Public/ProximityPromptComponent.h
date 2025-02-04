// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProximityPromptComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggered);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilityChanged, bool, NewVisible);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROOMBA_API UProximityPromptComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProximityPromptComponent();

	// Broadcast when the proximity prompt is triggered
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTriggered OnTriggered;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnVisibilityChanged OnVisibilityChanged;

	UPROPERTY(BlueprintReadWrite, Category="Config")
	float MaxActivationDistance;

	UPROPERTY(BlueprintReadOnly, Category="Info")
	bool Visible;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetVisibility(bool NewVisible);

	virtual void Trigger();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

private:

	float GetDistanceToPlayer();

	ACharacter* Character;
	
};
