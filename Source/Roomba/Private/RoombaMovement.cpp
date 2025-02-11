// Fill out your copyright notice in the Description page of Project Settings.


#include "RoombaMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARoombaMovement::ARoombaMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxCollider;

	RoombaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoombaMesh"));
	RoombaMesh->SetupAttachment(BoxCollider);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 600.0f;
	FloatingPawnMovement->Acceleration = 1200.0f;
	FloatingPawnMovement->Deceleration = 1200.0f;
	FloatingPawnMovement->TurningBoost = 8.0f;
}

// Called when the game starts or when spawned
void ARoombaMovement::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void ARoombaMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);*/

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARoombaMovement::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARoombaMovement::Look);
	}
	
}


void ARoombaMovement::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController()!= nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	
}

void ARoombaMovement::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (AController* PlayerController = GetController())
	{
		// Get the forward direction based on the controller's rotation
		FRotator ControllerRotation = PlayerController->GetControlRotation();
		FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);

		FVector MovementDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		MovementDirection.Z = 0;  // Zero out the Z component to prevent vertical movement

		FloatingPawnMovement->AddInputVector(MovementDirection);

	}

	
}



// Called every frame
void ARoombaMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform a raycast downward to check the ground distance
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000); // Raycast down 1000 units

	// Hit result for the raycast
	FHitResult HitResult;

	// Perform the raycast
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the Roomba itself in the collision check

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	// If the raycast hits something (ground), check the distance
	if (bHit)
	{
		float DistanceToGround = Start.Z - HitResult.ImpactPoint.Z;
		float TargetHeight = HitResult.ImpactPoint.Z + 90.0f; // The height should be at least 5 units above the ground
		
		FVector CurrentLocation = GetActorLocation();

		if (CurrentLocation.Z > TargetHeight + 90.0f)
		{
			CurrentLocation.Z = TargetHeight + 90.0f;  // Ensure the Z is set to at least 5 units above the ground
			SetActorLocation(CurrentLocation);
		}
		else

			{
			// Smoothly interpolate to the desired height
			CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, TargetHeight, DeltaTime, 1.5f);
			SetActorLocation(CurrentLocation);
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f); // Raycast line in green

	
}

