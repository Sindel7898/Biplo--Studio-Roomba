// Fill out your copyright notice in the Description page of Project Settings.


#include "RoombaMovement.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/Support/WinAdapter.h>

#include "BatteryMeterComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerSpline.h"
#include "ProximityPromptComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoombaMovement::ARoombaMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HoverDownRaycastLength = 5000.0f;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
	
	RoombaSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RoombaSkeletalMesh"));
	RoombaSkeletalMesh->SetupAttachment(RootComponent);

	CarryingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarryingMesh"));
	CarryingMesh->SetupAttachment(RoombaSkeletalMesh);
	CarryingMesh->SetVisibility(false);
	
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

	BatteryMeterComponent = CreateDefaultSubobject<UBatteryMeterComponent>(TEXT("BatteryMeterComponent"));


	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT(" Scene Component"));
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(SceneComponent);  // Attach to root or another component

}

// Called when the game starts or when spawned
void ARoombaMovement::BeginPlay()
{
	Super::BeginPlay();

	StoreMaxSpeed = FloatingPawnMovement->MaxSpeed;
	StoreDeceleration = FloatingPawnMovement->Deceleration;

	for (int i = 0; i < LevelsThatUseSpline.Num(); i++)
	{
		if (LevelsThatUseSpline[i] == UGameplayStatics::GetCurrentLevelName(GetWorld()))
		{
			CameraState = PlayerCameraState::AttachedToSpline;
			DoesLevelUseSpline = true;
			CanPlayerLook = false;
			PlayerSplineRef  = Cast<APlayerSpline>(UGameplayStatics::GetActorOfClass(GetWorld(),SplineActorClass));
			break;
		}
	}

	
	DefaultFOV = FollowCamera->FieldOfView;
	NewFOV = DefaultFOV;
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
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ARoombaMovement::OnDashInputChanged);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARoombaMovement::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARoombaMovement::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARoombaMovement::OnInteract);

	}
	
}




void ARoombaMovement::OnDashInputChanged(const FInputActionValue& InputActionValue)
{
	bool bIsDashing  = InputActionValue.Get<bool>();
	
	if (bIsDashing && !bIsCurrentlyDashing && CanPlayerMove && BatteryMeterComponent->GetBattery() >=4)
	{
		bIsCurrentlyDashing = true;
		
		FloatingPawnMovement->MaxSpeed = DashMaxSpeed;
		FloatingPawnMovement->Deceleration = 0.0f; 
		
		FloatingPawnMovement->Velocity += GetActorForwardVector() * DashMaxSpeed;

		BatteryMeterComponent->NegateStamina(BatteryMeterComponent->SpeedBoostMovementNegationAmount);
		
		FTimerHandle DashTimerHandle;
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ARoombaMovement::EndDash, DashDuration, false);
	}
}

void ARoombaMovement::EndDash()
{
	if (bIsCurrentlyDashing)
	{
		FloatingPawnMovement->StopMovementImmediately();
		FloatingPawnMovement->MaxSpeed = StoreMaxSpeed;
		FloatingPawnMovement->Velocity += GetActorForwardVector() * 600;
		FloatingPawnMovement->Deceleration = StoreDeceleration;
		bIsCurrentlyDashing = false;
	}
}


void ARoombaMovement::UpdateCarryingObject(UMaterialInterface* CarryingMaterial)
{
	// Realistically we're not going to have anything other than cables but if we are
	// such as in DARE then we might want to change this inventory to be an array of
	// generic items
	const bool IsHolding = CarryingCableCount > 0;

	if (CarryingMaterial != nullptr)
	{
		CarryingMesh->SetMaterial(0, CarryingMaterial);
	}
	
	CarryingMesh->SetVisibility(IsHolding);
	
}


void ARoombaMovement::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController()!= nullptr && CanPlayerLook)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	
}

void ARoombaMovement::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	FVector MovementDirection;
	FVector ForwardDirection;
	FVector RightDirection;
	
	if (AController* PlayerController = GetController())
	{
		if (CanPlayerMove && BatteryMeterComponent->GetBattery() > 0)
		{

			if (CanPlayerLook)
			{
				// Get the forward direction based on the controller's rotation
				FRotator ControllerRotation = PlayerController->GetControlRotation();
				ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
				RightDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);
			}
			else
			{
				ForwardDirection =  StaticForwardDirection;
				RightDirection   =  StaticRightDirection;
			}

			MovementDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
			MovementDirection.Z = 0;  // Zero out the Z component to prevent unwanted camera movement
		
			if (!MovementDirection.IsNearlyZero())
			{
				// Normalize direction to avoid scaling issues
				MovementDirection.Normalize();

				// Compute the target rotation based on movement direction
				FRotator TargetLookAtRotation = FRotationMatrix::MakeFromX(MovementDirection).Rotator();

				// Smoothly interpolate current rotation to the target rotation
				FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
				SetActorRotation(NewRotation);
			}
			
			BatteryMeterComponent->NegateStamina(BatteryMeterComponent->MovementNegationAmount * GetWorld()->GetDeltaSeconds());
			FloatingPawnMovement->AddInputVector(MovementDirection);
		}
	}
}


void ARoombaMovement::OnInteract(const FInputActionValue& InputActionValue)
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
			const bool Success = Comp->Trigger();
			if (Success)
			{
				OnPromptTriggered.Broadcast(FoundActor);
			}
		}
	}
}

// Called every frame
void ARoombaMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HoverPlayer(DeltaTime);
	ChangePlayerCamera();

	SceneComponent->SetWorldLocation(GetActorLocation());

	if (DoesLevelUseSpline)
	{
		if (PlayerSplineRef && CameraState == PlayerCameraState::AttachedToSpline)
		{

			CameraBoom->bInheritYaw = false;
			CameraBoom->bInheritPitch = false;
			CameraBoom->bDoCollisionTest = false;
			
			FVector CameraLocation = FollowCamera->GetComponentLocation();
			FRotator CameraRotation= FollowCamera->GetComponentRotation();
			
			float SplineKey = PlayerSplineRef->SplineComponent->FindInputKeyClosestToWorldLocation(CameraLocation);
			float SplineLength = PlayerSplineRef->SplineComponent->GetNumberOfSplinePoints() - 1;
			
			bool bAtEndOfSpline = (SplineKey >= SplineLength - 0.1f);

			if (bAtEndOfSpline)
			{
				CameraState = PlayerCameraState::AttachedToPlayer;
				return;
			}
			

			FVector SplineLocation = PlayerSplineRef->SplineComponent->FindLocationClosestToWorldLocation(GetActorLocation(),ESplineCoordinateSpace::World);
			
			FVector NewLocation =  FMath::VInterpTo(CameraLocation,SplineLocation,DeltaTime,CameraSplineInterSpeed);
			
			FRotator CalculatedLookat = UKismetMathLibrary::FindLookAtRotation(NewLocation,RoombaSkeletalMesh->GetComponentLocation());
			
			FRotator NewCameraRotation =  FMath::RInterpTo(CameraRotation,CalculatedLookat,DeltaTime,CameraSplineInterSpeed);

			 StaticForwardDirection = PlayerSplineRef->StaticForwardDirection;
		 	 StaticRightDirection = PlayerSplineRef->StaticRightDirection;
			 
			FollowCamera->SetWorldRotation(NewCameraRotation);
			FollowCamera->SetWorldLocation(NewLocation);		
		}
	}


	ChangePlayerCamera();
	
	
    	const float FOVInterpSpeed = 0.9; 
    	const float FOVTolerance = 0.1f;

	if (CameraState == PlayerCameraState::AttachedToPlayer)
	{
		if (bIsCurrentlyDashing)
		{
			if (!FMath::IsNearlyEqual(NewFOV, DashMaxFOV, FOVTolerance))
			{
				NewFOV = FMath::FInterpTo(NewFOV, DashMaxFOV, DeltaTime, FOVInterpSpeed);
				FollowCamera->SetFieldOfView(NewFOV);
    
				NewFOV = FMath::Clamp(NewFOV,0,120);
			}
		}
		else
		{
			if (!FMath::IsNearlyEqual(NewFOV, DefaultFOV, FOVTolerance))
			{
				NewFOV = FMath::FInterpTo(NewFOV, DefaultFOV, DeltaTime, FOVInterpSpeed);
				FollowCamera->SetFieldOfView(NewFOV);
			}
		}
	}
}



void ARoombaMovement::HoverPlayer(float DeltaTime)
{
	const float GravityStrength = 9.81f; // Gravity strength (m/s^2)
	
	// Perform a raycast downward to check the ground distance
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, HoverDownRaycastLength); // Raycast down 1000 units

	// Hit result for the raycast
	FHitResult HitResult;

	// Perform the raycast
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); 
 
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	// If the raycast hits something (ground), check the distance
	if (bHit)
	{
		float TargetHeight = HitResult.ImpactPoint.Z + HoverHeight;
		
		FVector CurrentLocation = GetActorLocation();

		CurrentLocation.Z -= GravityStrength * DeltaTime;
		
		if (CurrentLocation.Z > TargetHeight)
		{
			CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, TargetHeight, DeltaTime, HoverInterpulationSpeed);
		}
		// If the actor is below the target height, snap to the target height
		else if (CurrentLocation.Z < TargetHeight)
		{
			CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, TargetHeight, DeltaTime, HoverInterpulationSpeed);
		}
		
		SetActorLocation(CurrentLocation);

		FString InterpolationText = FString::Printf(TEXT("InterpolationSpeed: %f"), HoverInterpulationSpeed);
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, InterpolationText);
	}
}

void ARoombaMovement::ChangePlayerCamera()
 {

	if (CameraState == PlayerCameraState::AttachedToPlayer)
	{
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritPitch = true;
		CameraBoom->bDoCollisionTest = true;
		
		CanPlayerLook = true;

		FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetIncludingScale);
	
	}

	if (CameraState == PlayerCameraState::AtSpecifiedPosition)
	{
		GEngine->AddOnScreenDebugMessage(30, 2.0f, FColor::Red, TEXT("Switching Camera!"));
		CanPlayerLook = false;
		FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FVector CurrentLocation = FollowCamera->GetComponentLocation();
		
		FVector  NewLocation  = FMath::VInterpTo(CurrentLocation, TargetPosition, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);
		FRotator  NewRotation = FMath::RInterpTo(TargetRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);

		FTransform NewTransform = FTransform(NewRotation,NewLocation);
		FollowCamera->SetWorldTransform(NewTransform);
	}
}

void ARoombaMovement::MoveCameraTo(FVector Location, FRotator Rotation, float Length, float InInterpSpeed)
{

	FVector LastTargetPosition = TargetPosition;
	FRotator LastTargetRotation = TargetRotation;
	PlayerCameraState LastCameraState = CameraState;

	InterpolationSpeed = InInterpSpeed;
	TargetPosition = Location;
	TargetRotation = Rotation;
	CameraState = PlayerCameraState::AtSpecifiedPosition;
	BatteryMeterComponent->SetInActivationCamera(true);

	FTimerHandle Handle;

	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda(
		[this, LastTargetPosition, LastTargetRotation, LastCameraState]
	{
		ResetCamera(LastTargetPosition, LastTargetRotation, LastCameraState);
		
	}), Length, false);
	

	//ChangePlayerCamera();
}

void ARoombaMovement::ResetCamera(FVector LastTargetPosition, FRotator LastTargetRotation, PlayerCameraState LastCameraState)
{
	TargetPosition = LastTargetPosition;
	TargetRotation = LastTargetRotation;
	CameraState = LastCameraState;
	BatteryMeterComponent->SetInActivationCamera(false);
}

float ARoombaMovement::GetCurrentSpeed() 
{
	return FloatingPawnMovement->Velocity.Size();
}
