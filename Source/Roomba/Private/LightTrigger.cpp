// Fill out your copyright notice in the Description page of Project Settings.


#include "LightTrigger.h"

#include "SunLight.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightTrigger::ALightTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SunMoveToTrigger"));
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&ALightTrigger::OnComponentOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this,&ALightTrigger::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ALightTrigger::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),SunLightClass,FoundActors);

	for (auto FoundActor : FoundActors)
	{
		SunLightRef = Cast<ASunLight>(FoundActor);
		
		if (FoundActor)
		{
			break;
		}
	}
}

void ALightTrigger::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SunLightRef->SunState = SunLightTransionStates::TOSPECIFICPOSITION;
	SunLightRef->InterpolationSpeed = InterpolationRate;
	SunLightRef->TargetPosition = TargetSunPosition;
	
}

void ALightTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SunLightRef->SunState = SunLightTransionStates::Static;
	SunLightRef->InterpolationSpeed = 0;
	SunLightRef->TargetPosition = FRotator(0.0f,0.0f,0.0f);
	
}
