// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityPromptComponent.h"

#include "BatteryMeterComponent.h"
#include "RoombaMovement.h"
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
	bIsCollidingWithBoxComponent = false;

	Image = nullptr;

	StaminaRequired = 25.0f;
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
	if (IsValid(BoxComponent))
	{
		//GEngine->AddOnScreenDebugMessage(15,1,FColor::Green, "Added box component");

		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UProximityPromptComponent::OnComponentBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UProximityPromptComponent::OnComponentEndOverlap);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(15,1,FColor::Green, "No box component");

	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		RoombaMovement = Cast<ARoombaMovement>(PlayerPawn);
	}
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

void UProximityPromptComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr || OtherActor != PlayerPawn)
	{
		return; // Did not overlap with the player
	}
	
	GEngine->AddOnScreenDebugMessage(11,1,FColor::Green, "Overlap begin");
	bIsCollidingWithBoxComponent = true;
}

void UProximityPromptComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr || OtherActor != PlayerPawn)
	{
		return; // Did not overlap with the player
	}
	
	GEngine->AddOnScreenDebugMessage(12,1,FColor::Green, "Overlap end");
	bIsCollidingWithBoxComponent = false;
}

void UProximityPromptComponent::Trigger()
{
	if (!Visible)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "Proximity prompt has been triggered!!");
	
	bool CanTrigger = true;
	const bool DeductStamina = StaminaRequired > 0.0f && RoombaMovement != nullptr;
	if (DeductStamina)
	{
		float CurrentStamina = RoombaMovement->BatteryMeterComponent->GetBattery();
		if (CurrentStamina < StaminaRequired)
		{
			CanTrigger = false;
		}
	}

	if (CanTrigger)
	{
		if (DeductStamina)
		{
			RoombaMovement->BatteryMeterComponent->NegateStamina(-StaminaRequired);
		}
		OnTriggered.Broadcast();
	}
	
}

// Called every frame
void UProximityPromptComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool IsNearToInteraction = false;
	
	if (BoxComponent)
	{
		IsNearToInteraction = bIsCollidingWithBoxComponent;
		if (bIsCollidingWithBoxComponent)
		{
			GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "Colliding with box component?");
		}
	}
	else
	{
		float Distance = GetDistanceToPlayer();
		GEngine->AddOnScreenDebugMessage(7,1,FColor::Green, FString::SanitizeFloat(Distance));
		IsNearToInteraction = Distance < MaxActivationDistance;
	}
	
	if (IsNearToInteraction && Enabled)
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
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return -1.0f; // Return an invalid distance if the player pawn is null
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const FVector ProximityLocation = GetComponentLocation();

	return FVector::Distance(PlayerLocation, ProximityLocation);
}


