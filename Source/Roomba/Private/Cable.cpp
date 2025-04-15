// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable.h"

#include "RoombaMovement.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACable::ACable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(RootComponent);
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(CableComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&ACable::OverlapBegins);

}

// Called when the game starts or when spawned
void ACable::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACable::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(),0) && IsCableOnFloor == false && CableComponent->bHiddenInGame == false)
	{
		CableComponent->bAttachEnd = true;
		CableComponent->SetAttachEndTo(OtherActor,FName("CableConnectionPoint"));
	}
	
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(),0) && IsCableOnFloor)
	{
		ARoombaMovement* Player = Cast<ARoombaMovement>(OtherActor);
		if (Player->CarryingCableCount > 0)
		{
			return; // Already carrying a cable
		}
		
		UMaterialInterface* MainMaterial = CableComponent->GetMaterial(0);
		Player->CarryingCableCount++;
		Player->UpdateCarryingObject(MainMaterial);
		Destroy();
	}

	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(),0) && IsCableOnFloor == false && CableComponent->bHiddenInGame == true)
	{
		if (ARoombaMovement* Player = Cast<ARoombaMovement>(OtherActor))
		{
			if(Player->CarryingCableCount > 0)
			{

				UMaterialInterface* MainMaterial = CableComponent->GetMaterial(0);
				if (MainMaterial != Player->CarryingMesh->GetMaterial(0))
				{
					// Not the right cable
					return;
				}
				
				CableComponent->SetHiddenInGame(false);
				CableComponent->bAttachEnd = true;
				CableComponent->SetAttachEndTo(OtherActor,FName("CableConnectionPoint"));
				Player->CarryingCableCount--;
				Player->UpdateCarryingObject(nullptr);
			}
		}
		
	}
		
}
