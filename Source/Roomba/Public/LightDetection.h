#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealClient.h"
#include "LightDetection.generated.h"

UCLASS()
class  ALightDetection : public AActor
{
	GENERATED_BODY()
	
private:

	
	// Sets default values for this actor's properties
	ALightDetection();

	UFUNCTION()
	void CalculateTextureData();

	void ProcessRenderTexture(UTextureRenderTarget2D *Texture);
	
	TArray<FColor> PixelStorage;

	float CalculatedTexturData[2] ={0,0};
	
	float PixelChannelR = 0;
	float PixelChannelG = 0;
	float PixelChannelB = 0;
	
	float BrightnessOutput = 0;
	float CurrentPixelBrightness = 0;
	float TotalBlueness = 0;

	float TotalBrightness = 0;
	int   TotalPixelCount = 0;

	FRenderTarget *RenderTarget = nullptr;
	
	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D *DetectorTextureTop = nullptr;

	UPROPERTY(EditAnywhere)
	bool BLighting = false;
	bool IsPlayerInLight = false;
	bool IsPlayerInArtificialLight = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE bool GetBIsPlayerInLight() const {return IsPlayerInLight;}
	FORCEINLINE bool GetBIsPlayerInArtificialLight() const {return IsPlayerInArtificialLight;}

};
