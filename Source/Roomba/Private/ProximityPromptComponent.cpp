// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityPromptComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProximityPromptComponent::UProximityPromptComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxActivationDistance = 500.0f;
	Visible = false;
	Enabled = true;

	
}


void UProximityPromptComponent::SetEnabled(bool bEnabled)
{
	Enabled = bEnabled;
	
	if (Visible)
	{
		SetVisibility(false);	
	}
}

// Called when the game starts
void UProximityPromptComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UProximityPromptComponent::SetVisibility(bool NewVisible)
{
	if (NewVisible == Visible)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "set visible");

	Visible = NewVisible;
	OnVisibilityChanged.Broadcast(Visible);
}

void UProximityPromptComponent::Trigger()
{
	if (!Visible)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "Proximity prompt has been triggered!!");

	OnTriggered.Broadcast();
}

// Called every frame
void UProximityPromptComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	float Distance = GetDistanceToPlayer();
	GEngine->AddOnScreenDebugMessage(7,1,FColor::Green, FString::SanitizeFloat(Distance));

	if (Distance < MaxActivationDistance && Enabled)
	{
		SetVisibility(true);
	}
	else
	{
		SetVisibility(false);
	}
}

float UProximityPromptComponent::GetDistanceToPlayer()
{
	const FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	const FVector ProximityLocation = GetComponentLocation();

	return FVector::Distance(PlayerLocation, ProximityLocation);
}


