// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup configurations
	MaxThrust = 300.0f;
	Acceleration = 5.0f;

	CurrentForwardThrust = 0.0f;
	CurrentRightThrust = 0.0f;

	// Setup components
	DroneRootCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneRootCube"));
	DroneRootCube->SetAngularDamping(1);
	DroneRootCube->SetSimulatePhysics(true);
	DroneRootCube->SetHiddenInGame(true);
	DroneRootCube->SetEnableGravity(false);
	RootComponent = DroneRootCube;

	DroneStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneStaticMesh"));
	DroneStaticMesh->SetupAttachment(RootComponent);

	PrimarySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PrimarySpringArm"));
	PrimarySpringArm->SetupAttachment(RootComponent);
	PrimarySpringArm->TargetArmLength = 65.0f;
	PrimarySpringArm->bEnableCameraLag = true;
	
	PrimaryCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PrimaryCamera"));
	PrimaryCamera->SetupAttachment(PrimarySpringArm);
	PrimaryCamera->SetRelativeLocation(FVector(-150.0f, 0.0f, 155.0f));
	PrimaryCamera->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, -25.0f, 0.0f)));
	PrimaryCamera->bAutoActivate = true;

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADrone::OnInputChanged);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ADrone::OnInputChanged);

		
		
	}
	
}


float ADrone::GetDirectionSpeedMethod(float DeltaTime, float InputAxisValue, float CurrentSpeed)
{
	const float Alpha = (InputAxisValue + 1.0f) / 2.0f;
	const float Lerp = FMath::Lerp(-MaxThrust, MaxThrust, Alpha);
	const float InterpSpeed = Alpha * Acceleration;

	const float NewSpeed = FMath::FInterpTo(CurrentSpeed, Lerp, DeltaTime, InterpSpeed);
	return NewSpeed;
}


void ADrone::Move()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	if (Controller)
	{
		const float MoveRight    = MovementVector.X;
		const float MoveForward  = MovementVector.Y;
		const float ForwardSpeed = GetDirectionSpeedMethod(DeltaTime, MoveForward, CurrentForwardThrust);
		const float RightSpeed   = GetDirectionSpeedMethod(DeltaTime, MoveRight, CurrentRightThrust);

		CurrentRightThrust = RightSpeed;
		CurrentForwardThrust = ForwardSpeed;

		const FVector ForwardVector = DroneRootCube->GetForwardVector() * CurrentForwardThrust;
		const FVector RightVector = DroneRootCube->GetRightVector() * CurrentRightThrust;

		MoveResult = ForwardVector + RightVector;
	}
}

void ADrone::OnInputChanged(const FInputActionValue& InputActionValue)
{
	MovementVector = InputActionValue.Get<FVector2D>();
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	Move();
	
	DroneRootCube->SetPhysicsLinearVelocity(MoveResult);
}

