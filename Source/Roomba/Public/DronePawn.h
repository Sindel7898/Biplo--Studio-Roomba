// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DronePawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UBatteryMeterComponent;

UCLASS()
class ROOMBA_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*// TODO DESCRIPTION
	FVector GetMovementVelocityMethod(float DeltaTime);*/

	// TODO DESCRIPTION
	float GetDirectionSpeedMethod(float DeltaTime, float InputAxisValue, float CurrentSpeed);

	UPROPERTY(EditAnywhere,Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere,Category = Input)
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere,Category = "Energy")
	UBatteryMeterComponent* BatteryMeterComponent;
	
	// Moving input has changed
	void OnMoveInputChanged(const FInputActionValue& InputActionValue);

	// Dash input has changed
	void OnDashInputChanged(const FInputActionValue& InputActionValue);

	void OnInteract(const FInputActionValue& InputActionValue);
	
	// Move, called every frame
	void Move(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Root cube is the actual physics parts of the drone. This allows it to not be tied
	// into what mesh the artists are designing and their mass, scale, etc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UStaticMeshComponent* DroneRootCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UStaticMeshComponent* DroneStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	USpringArmComponent* PrimarySpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneComponents")
	UCameraComponent* PrimaryCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConfiguration")
	float MaxThrust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConfiguration")
	float Acceleration;

	// TODO DESCRIPTION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LiveStats");
	float CurrentForwardThrust;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LiveStats");
	float CurrentRightThrust;


private:
	FVector MoveResult;
	FVector2D MovementVector;

	

	
};
