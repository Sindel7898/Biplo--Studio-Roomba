// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba/Public/BatteryMeterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Roomba/Public/LightDetection.h"


// Sets default values for this component's properties
UBatteryMeterComponent::UBatteryMeterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DefaultBatteryLevel = 100.0f;
	BatteryLevel = DefaultBatteryLevel;
// test commit
}


// Called when the game starts
void UBatteryMeterComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),LightBPClass,FoundActors);

	for (AActor* Actor : FoundActors)
	{
		LightDetectionRef = Cast<ALightDetection>(Actor);

		if (LightDetectionRef)
		{
			break;
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::IncreaseStamina, IncreaseRate, true);

}



// Called every frame
void UBatteryMeterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// Adjust the timer rate dynamically based on light detection
	if (LightDetectionRef && LightDetectionRef->GetBIsPlayerInLight())
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRateInLight)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::IncreaseStamina, IncreaseRateInLight, true);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRate)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::IncreaseStamina, IncreaseRate, true);
		}
	}
	

	FString BatteryData = FString::Printf(TEXT("Battery = %f"),BatteryLevel);
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Green,BatteryData);

	FString Batteryincreasestring = FString::Printf(TEXT("Battery Increase Speed = %f"),GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle));
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Green,Batteryincreasestring);
}

void UBatteryMeterComponent::IncreaseStamina() 
{
	BatteryLevel++;
	
	if (BatteryLevel >= 100)
	{
		BatteryLevel = 100;
	}
}

void UBatteryMeterComponent::NegateStamina( float Amount) 
{
	BatteryLevel -= Amount;
	
	if (BatteryLevel <=  0)
	{
		BatteryLevel = 0;
	}
}
