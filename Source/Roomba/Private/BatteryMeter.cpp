// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba/Public/BatteryMeter.h"
#include "Kismet/GameplayStatics.h"
#include "Roomba/Public/LightDetection.h"


// Sets default values for this component's properties
UBatteryMeter::UBatteryMeter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UBatteryMeter::BeginPlay()
{
	Super::BeginPlay();
	BatteryLevel = 100;

	// search through all the actors in the scene to fidn the light detection actor
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
	// slowly increase the battery 
	GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeter::IncreaseStamina, IncreaseRate, true);

}



// Called every frame
void UBatteryMeter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// Adjust the timer rate dynamically based on light detection
	if (LightDetectionRef && LightDetectionRef->GetBIsPlayerInLight())
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRateInLight)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeter::IncreaseStamina, IncreaseRateInLight, true);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRate)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeter::IncreaseStamina, IncreaseRate, true);
		}
	}
	
    ///Debug logs///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FString BatteryData = FString::Printf(TEXT("Battery = %f"),BatteryLevel);
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Green,BatteryData);

	FString Batteryincreasestring = FString::Printf(TEXT("Battery Increase Speed = %f"),GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle));
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Green,Batteryincreasestring);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


void UBatteryMeter::IncreaseStamina() 
{
	BatteryLevel += 0.2;

	// cap battery level to 100
	if (BatteryLevel >= 100)
	{
		BatteryLevel = 100;
	}
}

void UBatteryMeter::NegateStamina( float NegateAmount) 
{
	BatteryLevel +=NegateAmount;
	// cap battery level to 0
	if (BatteryLevel <=  0)
	{
		BatteryLevel = 0;
	}
}
