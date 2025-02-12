// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSwitcher.h"
#include "RoombaMovement.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelSwitcher::ALevelSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));

	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this,&ALevelSwitcher::OverlapBegins);

}

// Called when the game starts or when spawned
void ALevelSwitcher::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	cameramanager->StartCameraFade(1, 0, 1.5, FLinearColor::Black, false, true);


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerCharacter,FoundActors);

	for (AActor* Actor : FoundActors)
	{
		RoombaCharacter = Cast<ARoombaMovement>(Actor);

		if (RoombaCharacter)
		{
			break;
		}
	}

	
}



void ALevelSwitcher::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if ( RoombaCharacter && OtherActor == RoombaCharacter && LevelToLoad != "")
	{
		APlayerCameraManager * cameramanager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		cameramanager->StartCameraFade(0, 1, 1.5, FLinearColor::Black, false, true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALevelSwitcher::LoadNextLevel, 1.5f, false);
	}
	
}

void ALevelSwitcher::LoadNextLevel()
{
	UGameplayStatics::OpenLevel(this, LevelToLoad);
}



