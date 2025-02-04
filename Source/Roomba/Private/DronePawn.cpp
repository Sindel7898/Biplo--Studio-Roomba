// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"

#include "BatteryMeterComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup configurations
	MaxThrust = 300.0f;
	Acceleration = 5.0f;

	CurrentForwardThrust = 0.0f;
	CurrentRightThrust = 0.0f;

	MovementVector = FVector2D(0.0f, 0.0f);
	
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

	BatteryMeterComponent = CreateDefaultSubobject<UBatteryMeterComponent>(TEXT("BatteryMeterComponent"));

}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADronePawn::OnMoveInputChanged);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ADronePawn::OnMoveInputChanged);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ADronePawn::OnDashInputChanged);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ADronePawn::OnDashInputChanged);

		
	}
	
}


float ADronePawn::GetDirectionSpeedMethod(float DeltaTime, float InputAxisValue, float CurrentSpeed)
{
	const float Alpha = (InputAxisValue + 1.0f) / 2.0f;
	const float Lerp = FMath::Lerp(-MaxThrust, MaxThrust, Alpha);
	const float InterpSpeed = Alpha * Acceleration;

	const float NewSpeed = FMath::FInterpTo(CurrentSpeed, Lerp, DeltaTime, InterpSpeed);
	return NewSpeed;
}


void ADronePawn::OnDashInputChanged(const FInputActionValue& InputActionValue)
{
	bool DashValue = InputActionValue.Get<bool>();
	if (DashValue)
	{
		GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "On");

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5,1,FColor::Green, "Off");

	}
}

void ADronePawn::OnMoveInputChanged(const FInputActionValue& InputActionValue)
{
	MovementVector = InputActionValue.Get<FVector2D>();
}

void ADronePawn::Move(float DeltaTime)
{
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

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	Move(DeltaTime);

	if (MoveResult.Length() >= 0.90f)
	{
		BatteryMeterComponent->NegateStamina(1.0f * DeltaTime);
	}
	
	DroneRootCube->SetPhysicsLinearVelocity(MoveResult);
}

