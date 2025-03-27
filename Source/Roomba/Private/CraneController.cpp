// Fill out your copyright notice in the Description page of Project Settings.


#include "CraneController.h"

#include "Crane.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RoombaMovement.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACraneController::ACraneController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ControlDeskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControlDeskMesh"));
	ControlDeskMesh->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void ACraneController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACraneController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ControllerMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveCraneUp       , ETriggerEvent::Triggered, this, &ACraneController::MoveUp);
		EnhancedInputComponent->BindAction(MoveCraneDown     , ETriggerEvent::Triggered, this, &ACraneController::MoveDown);
		EnhancedInputComponent->BindAction(MoveCraneLeft     , ETriggerEvent::Triggered, this, &ACraneController::RotateLeft);
		EnhancedInputComponent->BindAction(MoveCraneRight    , ETriggerEvent::Triggered, this, &ACraneController::RotateRight);
	//	EnhancedInputComponent->BindAction(SwitchBackToPlayer, ETriggerEvent::Completed, this, &ACraneController::SwitchToPlayer);

	}

}


// Called every frame
void ACraneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraneController::MoveUp()
{
	CraneRef->MoveCraneUp(1.0f);
}

void ACraneController::MoveDown()
{
	CraneRef->MoveCraneDown(1.0f);
}

void ACraneController::RotateLeft()
{
	CraneRef->RotateCraneLeft(1.0f);
}

void ACraneController::RotateRight()
{
	CraneRef->RotateCraneRight(1.0f);
}

void ACraneController::ClearInput()
{
	TSubclassOf<ARoombaMovement> PlayerClass = ARoombaMovement::StaticClass();
	TArray<AActor*> FoundPlayerActor;
	
	//find other character
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerClass,FoundPlayerActor);

	for (AActor* actors : FoundPlayerActor)
	{
		ARoombaMovement* PlayerRef = Cast<ARoombaMovement>(actors);
		
		if (PlayerRef)
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->ClearAllMappings(); 
					Subsystem->AddMappingContext(PlayerRef->GetMappingContext(), 0);
					GetController()->Possess(PlayerRef);
				}
			}
		
	}

				
}


/*
void ACraneController::SwitchToPlayer()
{
	
	TSubclassOf<ARoombaMovement> PlayerClass = ARoombaMovement::StaticClass();
	TArray<AActor*> FoundPlayerActor;
	
	//find other character
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),PlayerClass,FoundPlayerActor);

	for (AActor* actors : FoundPlayerActor)
	{
	   ARoombaMovement* PlayerRef = Cast<ARoombaMovement>(actors);
		
		if (PlayerRef)
		{
			if (GetController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Switched to Player."));
				
				if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
				{
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
					{
						Subsystem->ClearAllMappings(); 
						Subsystem->AddMappingContext(PlayerRef->GetMappingContext(), 0);
						GetController()->Possess(PlayerRef);
					}
				}
				
				break; 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller is null. Cannot possess Player."));
			}
		}
	}
}
*/
