// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyComponent.generated.h"

// obsolete: needs deleting..

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMBA_API UEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnergyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Current energy of the component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	float CurrentEnergyAmount;

	// The energy that the component starts with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float DefaultEnergyAmount;

	// The minimum amount of energy the component can go to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float MinEnergyAmount;

	// The maximum amount of energy the component can go to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergyAmount;

	// When in a shadow, how much should the energy increase by?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float EnergyIncreaseAmount;

	// When not in a shadow, how much should the energy increase by?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float EnergyDecreaseAmount;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function that can be used by the Shadow detection code to adjust the energy meter
	void EnergyUpdate(bool IsInShadow);
};
