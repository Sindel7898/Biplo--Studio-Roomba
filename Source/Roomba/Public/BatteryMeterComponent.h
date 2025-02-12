// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BatteryMeterComponent.generated.h"


class ALightDetection;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBatteryMeterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBatteryMeterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void IncreaseStamina();
	
	float BatteryLevel;
	
	FTimerHandle StaminaIncreaseHandle;
	
	UPROPERTY()
	ALightDetection* LightDetectionRef;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultBatteryLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBatteryLevel;
	
	UPROPERTY(EditAnywhere)
	float IncreaseRate = 0.2f;

	UPROPERTY(EditAnywhere)
	float IncreaseRateInLight = 0.1f;

	
	UPROPERTY(EditAnywhere)
	float MovementNegationAmount = -15.0f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LightBPClass;
	
	void NegateStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	float GetBatteryLevel() {return BatteryLevel  / MaxBatteryLevel;} 

};
