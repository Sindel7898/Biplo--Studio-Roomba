// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

#include "BatteryMeterComponent.h"
#include "RoombaMovement.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));

	
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this,&ACheckPoint::OverlapBegins);

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPoint::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector ActorLocation = GetActorLocation();
	FString SaveLocationText = FString::Printf(TEXT("new Checkpoint location = X: %f, Y: %f, Z: %f"), ActorLocation.X, ActorLocation.Y, ActorLocation.Z);

	GEngine->AddOnScreenDebugMessage(10,1,FColor::Red,SaveLocationText);

	
	if (ARoombaMovement* PlayerRef = Cast<ARoombaMovement>(OtherActor))
	{
		PlayerRef->BatteryMeterComponent->SpawnPosition = GetActorLocation();
	}
}