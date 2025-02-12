// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RoombaMovement.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UFloatingPawnMovement;
class UBatteryMeterComponent;

UENUM()
enum PlayerCamerastate
{
	AtSpecifiedPosition UMETA(DisplayName = "AtSpecifiedPosition"),
	AttachedToPlayer UMETA(DisplayName = "AttachedToPlayer"),
};


UCLASS()
class ROOMBA_API ARoombaMovement : public APawn
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* InteractAction;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
public:
	// Sets default values for this character's properties
	ARoombaMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBatteryMeterComponent* BatteryMeterComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& InputActionValue);

	/** Called for looking input */
	UFUNCTION()
	void Look(const FInputActionValue& Value);


	void HoverPlayer();
	void ChangePlayerCamera();

	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnDashInputChanged(const FInputActionValue& InputActionValue);


	UPROPERTY(EditAnywhere)
    float  DashMaxSpeed;
	float  StoreMaxSpeed;
	
	FVector DefaultCameraPosition;
	float InterpolationSpeed ;
	FVector TargetPosition ;
	FRotator TargetRotation ;

	FVector StaticForwardDirection;
	FVector StaticRightDirection;
	bool CanPlayerLook = true;

	PlayerCamerastate CameraState = PlayerCamerastate::AttachedToPlayer;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RoombaMesh; 
};
