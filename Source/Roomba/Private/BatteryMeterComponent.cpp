// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba/Public/BatteryMeterComponent.h"

#include "RoombaMovement.h"
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
	MaxBatteryLevel = 100.0f;
	bInLight = false;
// test commit
}

void UBatteryMeterComponent::SetInLight(const bool bNewInLight)
{
	if (bInLight != bNewInLight)
	{
		bInLight = bNewInLight;
		InShadowChanged.Broadcast(not bInLight);
	}
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
			SpawnPosition = LightDetectionRef->GetActorLocation();
			break;
		}
	}

	
	InShadowChanged.Broadcast(not bInLight);

	 PlayerRef = Cast<ARoombaMovement>(GetOwner());

}



// Called every frame
void UBatteryMeterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool InArtificialLight = LightDetectionRef && LightDetectionRef->GetBIsPlayerInArtificialLight();
	const bool InRegularLight = LightDetectionRef && LightDetectionRef->GetBIsPlayerInLight();
	const bool InAnyLight =  InArtificialLight || InRegularLight;

	if (LightDetectionRef && InArtificialLight)
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRateInArtificialLight)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::IncreaseStamina2, IncreaseRateInArtificialLight, true);
		}
	}
	
	// Adjust the timer rate dynamically based on light detection
	if (LightDetectionRef && InRegularLight && !InArtificialLight)
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != IncreaseRateInLight)
		{
	
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::IncreaseStamina2, IncreaseRateInLight, true);
		}
	}
	else if (LightDetectionRef && !InRegularLight)
	{
		if (GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle) != DecreaseRateWhileInShadow)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaIncreaseHandle, this, &UBatteryMeterComponent::DecreaseStaminaInShadow, DecreaseRateWhileInShadow, true);

		}
	}
	
	if (BatteryLevel <= 0 && !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		if (PlayerRef)
		{
			//TODO: Check velocity is 0
			const float VelocityLength = PlayerRef->GetRootComponent()->GetComponentVelocity().Length();

			//TODO: Check if in the shadow, and then kill
			const bool IsInShadow = !InAnyLight;

			if (IsInShadow && FMath::IsNearlyEqual(VelocityLength, 0.0f, 0.01f))
			{
				APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				cameramanager->StartCameraFade(0, 1, 1.5, FLinearColor::Black, true, true);
				PlayerRef->CanPlayerLook = false;
				PlayerRef->CanPlayerMove = false;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBatteryMeterComponent::RespawnPlayer, 1.5f, false);
			}
		}
	}
	
	FString BatteryData = FString::Printf(TEXT("Battery = %f"),BatteryLevel);
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Green,BatteryData);

	FString Batteryincreasestring = FString::Printf(TEXT("Battery Increase Speed = %f"),GetWorld()->GetTimerManager().GetTimerRate(StaminaIncreaseHandle));
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Green,Batteryincreasestring);
	
	SetInLight(InAnyLight);
	
}


void UBatteryMeterComponent::IncreaseStamina2() 
{
	FString BatteryincrEQFRWReasestring = FString::Printf(TEXT("INCREASE STAMINA CALLED "));
	GEngine->AddOnScreenDebugMessage(23,1,FColor::Green,BatteryincrEQFRWReasestring);
	
	BatteryLevel++;
	
	if (BatteryLevel >= 100)
	{
		BatteryLevel = 100;
	}
}

void UBatteryMeterComponent::DecreaseStaminaInShadow() 
{
	BatteryLevel--;
}
void UBatteryMeterComponent::NegateStamina( float Amount) 
{
	BatteryLevel += Amount;
}

void UBatteryMeterComponent::RespawnPlayer() 
{
	APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (PlayerRef && cameramanager )
	{
		PlayerRef->SetActorLocation(SpawnPosition);
		BatteryLevel = 100;
		cameramanager->StartCameraFade(1, 0, 1.5, FLinearColor::Black, true, true);
		PlayerRef->CanPlayerMove = true;

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}