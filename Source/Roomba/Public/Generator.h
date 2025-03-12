
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

class ACable;
class UBoxComponent;
class UStaticMeshComponent;
class AGeneratorSwitch;

UCLASS()
class ROOMBA_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	
	UPROPERTY(EditAnywhere)
	UMeshComponent* GeneratorMesh;

	UPROPERTY(EditAnywhere)
	UMaterial*  EnabledMaterial;
	UPROPERTY(EditAnywhere)
	UMaterial*  DisabledMaterial;
	
	UPROPERTY(EditAnywhere)
	ACable* Cable;

	UPROPERTY(EditAnywhere)
	AGeneratorSwitch* GeneratorSwitch;
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool IsConnectedToRope = false;

	void SwitchToEnabled();
	void SwitchToDisabled();
};
