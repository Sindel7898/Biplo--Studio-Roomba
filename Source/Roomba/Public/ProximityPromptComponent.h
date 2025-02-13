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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float MaxActivationDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	bool Visible;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	bool Enabled;
	
	UFUNCTION(BlueprintCallable, Category="Config")
	void SetEnabled(bool bEnabled);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetVisibility(bool NewVisible);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Trigger();

private:

	float GetDistanceToPlayer();
};
