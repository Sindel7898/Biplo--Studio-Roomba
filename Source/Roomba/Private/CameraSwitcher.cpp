// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSwitcher.h"

#include "RoombaCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraSwitcher::ACameraSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraSwitchTrigger"));

	

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&ACameraSwitcher::OnComponentOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this,&ACameraSwitcher::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ACameraSwitcher::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerCharacter,FoundActors);

	for (AActor* Actor : FoundActors)
	{
		RoombaCharacter = Cast<ARoombaCharacter>(Actor);

		if (RoombaCharacter)
		{
			break;
		}
	}
}

void ACameraSwitcher::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (RoombaCharacter)
	{
		RoombaCharacter->cameraState = CameraState::AtSpecifiedPosition;
		RoombaCharacter->InterpolationSpeed = InterpolationRate;
		RoombaCharacter->TargetPosition = TargetCameraPosition;
		RoombaCharacter->TargetRotation = TargetCameraRotation;	

	}
	
}

void ACameraSwitcher::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (RoombaCharacter)
	{
		RoombaCharacter->cameraState = CameraState::AttachedToPlayer;

	}
}

// Called every frame
void ACameraSwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

