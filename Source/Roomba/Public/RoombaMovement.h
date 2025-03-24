// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RoombaMovement.generated.h"

class UCapsuleComponent;
class USphereComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UFloatingPawnMovement;
class UBatteryMeterComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UWidgetComponent;

UENUM()
enum PlayerCameraState 
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

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* SpeedBoostAction;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	
public:
	// Sets default values for this character's properties
	ARoombaMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBatteryMeterComponent* BatteryMeterComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFloatingPawnMovement* FloatingPawnMovement;
	
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


	void HoverPlayer(float DeltaTime);
	void ChangePlayerCamera();

	UFUNCTION(BlueprintCallable)
	void MoveCameraTo(FVector Location, FRotator Rotation);
	
	UFUNCTION(BlueprintCallable)
	void ResetCamera();

	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnDashInputChanged(const FInputActionValue& InputActionValue);
	void EndDash();

	UPROPERTY(EditAnywhere)
	float  HoverHeight = 100;
	
	UPROPERTY(EditAnywhere)
    float  DashMaxSpeed = 500;
	float  StoreMaxSpeed;
	float  StoreDeceleration;

	bool bIsCurrentlyDashing = false;
	UPROPERTY(EditAnywhere)
	float DashDuration = 0.6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverInterpulationSpeed = 1.0f;
	
	FVector DefaultCameraPosition;
	float InterpolationSpeed ;
	FVector TargetPosition ;
	FRotator TargetRotation ;

	FVector StaticForwardDirection;
	FVector StaticRightDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanPlayerLook = true;

	PlayerCameraState CameraState = PlayerCameraState::AttachedToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* RoombaSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverDownRaycastLength;

	bool CanPlayerMove = true;

	FORCEINLINE UInputMappingContext* GetMappingContext(){return DefaultMappingContext;}

	float CableCount = 0;
};
