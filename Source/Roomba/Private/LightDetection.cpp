// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba/Public/LightDetection.h"

#include "Engine/TextureRenderTarget2D.h"


// Sets default values
ALightDetection::ALightDetection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ALightDetection::BeginPlay()
{
	Super::BeginPlay();

	if (DetectorTextureTop)
	{
		int32 TextureWidth = DetectorTextureTop->SizeX;
		int32 TextureHeight = DetectorTextureTop->SizeY;
		PixelStorage.Reserve(TextureWidth * TextureHeight); // Pre-allocate memory
	}
	
}

int FrameCount = 0;

// Called every frame
void ALightDetection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FrameCount++;
	
	if (FrameCount == 10)
	{
		CalculateTextureData();
		//Debug
		FString BrightnessString = FString::Printf(TEXT("Brightness = %f"),TotalBrightness);
		GEngine->AddOnScreenDebugMessage(3,1,FColor::Yellow,BrightnessString);

		FString BluenessString = FString::Printf(TEXT("Blueness = %f"),TotalBlueness);
		GEngine->AddOnScreenDebugMessage(4,1,FColor::Blue,BluenessString);

		FString BrightnessBoolString = FString::Printf(TEXT(" Past Light Thresh = %d"),IsPlayerInLight);
		GEngine->AddOnScreenDebugMessage(5,1,FColor::Red,BrightnessBoolString);
		///////////////////////////////////////////////////////////////////////////////////////////
		
		if (TotalBrightness > 60.0f)
		{
			IsPlayerInLight = true;
		}
		else
		{
			IsPlayerInLight = false;
		}


		if (TotalBlueness > 120.0f)
		{
			IsPlayerInArtificialLight = true;
		}
		else
		{
			IsPlayerInArtificialLight = false;

		}
		
		FrameCount = 0;
	}
}


void  ALightDetection::CalculateTextureData()
{
	if (DetectorTextureTop  == nullptr) {
		return;
	}
	
	// Reset our values for the next brightness test
	TotalBrightness = 0.0f;
	TotalBlueness = 0.0f;

	TotalPixelCount = 0;
	
	// Process our top and bottom RenderTextures
	ProcessRenderTexture(DetectorTextureTop);
	
	//Average the brightness of all the pixel
	if (TotalPixelCount > 0)
	{
		TotalBrightness =  TotalBrightness / static_cast<float>(TotalPixelCount);
		TotalBlueness   =  TotalBlueness  / static_cast<float>(TotalPixelCount);
	}
	
}


void ALightDetection::ProcessRenderTexture(UTextureRenderTarget2D* Texture)
{
	// Read the pixels from our RenderTexture and store the data into our color array
	RenderTarget = Texture->GameThread_GetRenderTargetResource();

	
	RenderTarget->ReadPixels(PixelStorage);


	//iterate through every pixel  retrieved and find the brightest pixel
	for (int PixelNum = 0; PixelNum <PixelStorage.Num(); PixelNum++) {
		
		PixelChannelR = PixelStorage[PixelNum].R;
		PixelChannelG = PixelStorage[PixelNum].G;
		PixelChannelB = PixelStorage[PixelNum].B;
		
		CurrentPixelBrightness = ((0.299 * PixelChannelR) + (0.587 * PixelChannelG) + (0.114 * PixelChannelB));
		TotalBrightness += CurrentPixelBrightness;

		TotalBlueness += PixelChannelB;

	}

	TotalPixelCount += PixelStorage.Num();
}


