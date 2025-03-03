// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BatteryMeterComponent.generated.h"



class ALightDetection;
class ARoombaMovement;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInShadowChanged, bool, NewInShadow);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBatteryMeterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBatteryMeterComponent();

private:

	void SetInLight(const bool bNewInLight);

	bool bInLight;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void IncreaseStamina2();

	UFUNCTION()
	void DecreaseStaminaInShadow();

	float BatteryLevel;
	
	FTimerHandle StaminaIncreaseHandle;
	
	UPROPERTY()
	ALightDetection* LightDetectionRef;


	UPROPERTY()
	ARoombaMovement* PlayerRef;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RespawnPlayer();

	// Broadcast when the proximity prompt is triggered
	UPROPERTY(BlueprintAssignable, Category="Events")
	FInShadowChanged InShadowChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultBatteryLevel = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBatteryLevel = 100;
	
	UPROPERTY(EditAnywhere)
	float DecreaseRateWhileInShadow = 0.1f;

	UPROPERTY(EditAnywhere)
	float IncreaseRateInLight = 0.05f;
	
	UPROPERTY(EditAnywhere)
	float IncreaseRateInArtificialLight = 0.099f;

	UPROPERTY(EditAnywhere)
	float MovementNegationAmount = -3.0f;
	
	UPROPERTY(EditAnywhere)
	float SpeedBoostMovementNegationAmount = -35.0f;
	
	UPROPERTY(EditAnywhere)
	float NegateInShadowAmount = -5.0f;

	UPROPERTY(EditAnywhere)
	float SpeedBoostCuncurrentDrainRate = -5.0f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LightBPClass;

	UFUNCTION(BlueprintCallable)
	void NegateStamina(float Amount);

	FTimerHandle TimerHandle;

	
	UFUNCTION(BlueprintCallable)
	float GetBatteryLevel() {return BatteryLevel  / MaxBatteryLevel;} 

	FVector SpawnPosition;
};
