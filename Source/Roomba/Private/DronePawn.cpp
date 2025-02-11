// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"

#include "BatteryMeterComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProximityPromptComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup configurations
	MaxThrust = 300.0f;
	Acceleration = 5.0f;

	DashLength = 0.5f;
	TimeSpentDashing = 0.0f;

	CurrentForwardThrust = 0.0f;
	CurrentRightThrust = 0.0f;
	bIsDashing = false;

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
	PrimarySpringArm->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	BatteryMeterComponent = CreateDefaultSubobject<UBatteryMeterComponent>(TEXT("BatteryMeterComponent"));

}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	DefaultCameraPosition = PrimaryCamera->GetComponentLocation();
	PrimaryCamera->AttachToComponent(PrimarySpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);

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
	


	if (CameraState == PlayerCamerastate::AttachedToPlayer)
	{
		CanPlayerLook = true;

		if (!PrimaryCamera->GetAttachParent()) 
		{
			PrimaryCamera->AttachToComponent(PrimarySpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		FVector CurrentLocation = PrimaryCamera->GetComponentLocation();

		FVector NewLocation = FMath::VInterpTo(CurrentLocation, DefaultCameraPosition, DeltaTime, InterpolationSpeed);
		//PrimaryCamera->SetWorldLocation(NewLocation);
	}

	if (CameraState == PlayerCamerastate::AtSpecifiedPosition)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Switching Camera POsitrion!"));
		CanPlayerLook = false;
		PrimaryCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FVector CurrentLocation = PrimaryCamera->GetComponentLocation();
		
		FVector  NewLocation  = FMath::VInterpTo(CurrentLocation, TargetPosition, DeltaTime, InterpolationSpeed);
		FRotator  NewRotation = FMath::RInterpTo(TargetRotation, TargetRotation, DeltaTime, InterpolationSpeed);

		FTransform NewTransform = FTransform(NewRotation,NewLocation);
		PrimaryCamera->SetWorldTransform(NewTransform);
	}
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

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADronePawn::Look);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ADronePawn::OnDashInputChanged);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ADronePawn::OnDashInputChanged);

		// Trigger
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADronePawn::OnInteract);

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
	bIsHoldingDash = DashValue;
	
	if (DashValue)
	{
		bIsDashing = true;
		TimeSpentDashing = 0.0f;
	}
	
}

void ADronePawn::OnInteract(const FInputActionValue& InputActionValue)
{
	// Loop through proximity prompts

	TArray<AActor*> InteractionActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Interaction", InteractionActors);

	for (int i = 0; i < InteractionActors.Num(); i++)
	{
		AActor* FoundActor = InteractionActors[i];
		UProximityPromptComponent* Comp = Cast<UProximityPromptComponent>(FoundActor->GetComponentByClass(UProximityPromptComponent::StaticClass()));
		if (Comp)
		{
			Comp->Trigger();
		}
	}

	// Call Trigger
}

void ADronePawn::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CanPlayerLook)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
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

		float ForwardSpeed = GetDirectionSpeedMethod(DeltaTime, MoveForward, CurrentForwardThrust);
		float RightSpeed   = GetDirectionSpeedMethod(DeltaTime, MoveRight, CurrentRightThrust);
		
		CurrentRightThrust = RightSpeed;
		CurrentForwardThrust = ForwardSpeed;

		if (bIsHoldingDash && !bIsDashing)
		{
			// Dash over, now just make it double
			// Add dash
			ForwardSpeed *= 2.0f;
			RightSpeed *= 2.0f;
		}

		if (bIsDashing)
		{
			// Make MoveResult quicker
			TimeSpentDashing+= DeltaTime;
			
			// Currently dashing, keep going
			// Add dash
			ForwardSpeed *= 3.5f;
			RightSpeed *= 3.5f;
			
			if (TimeSpentDashing >= DashLength)
			{
				bIsDashing = false;
			}
		}

		 FVector ForwardVector;
		 FVector RightVector;
		
		if (CanPlayerLook)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			FVector CameraForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// get right vector 
			FVector CameraRightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			ForwardVector = CameraForwardDirection * ForwardSpeed;
			RightVector = CameraRightDirection * RightSpeed;
			
		}
		else
		{
			ForwardVector = StaticForwardDirection * ForwardSpeed;
		    RightVector = StaticRightDirection * RightSpeed;
		}
		
		MoveResult = ForwardVector + RightVector;
		
		DroneRootCube->SetPhysicsLinearVelocity(MoveResult);

	}
}

