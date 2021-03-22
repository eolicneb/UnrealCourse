// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningFlare.h"

// Sets default values for this component's properties
UOpeningFlare::UOpeningFlare()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpeningFlare::BeginPlay()
{
	Super::BeginPlay();

	FlareComponent = GetOwner()->FindComponentByClass<ULightComponent>();
	if (!FlareComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpeningFlare component but no ULightComponent!"),
			*GetOwner()->GetName());
	}
	else
	{
		InitialBright = FlareComponent->ComputeLightBrightness();
		UE_LOG(LogTemp, Warning, TEXT("Initial brightness of %s is %f"),
			*GetOwner()->GetName(), InitialBright);
	}
	Explode();

}


// Called every frame
void UOpeningFlare::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Burning)
	{
		UpdateBrightness(DeltaTime);
	}
}

void UOpeningFlare::Explode()
{
	if (Burning) { return; }
	UE_LOG(LogTemp, Error, TEXT("We are about to EXPLODE!!!"));
	Burning = true;
	IgnitionTime = GetWorld()->GetTimeSeconds();
}

void UOpeningFlare::UpdateBrightness(float DeltaTime)
{
	if (!Burning || GetWorld()->GetTimeSeconds() - IgnitionTime < BurstDelay) { return; }
	if (!Extinguishing)
	{
		UE_LOG(LogTemp, Error, TEXT("BBOOOOOOMMMMMMMMM!!!!!!"));
		CurrentBright = BurstBright;
		FlareComponent->SetLightBrightness(CurrentBright);
		Extinguishing = true;
	}
	else
	{
		CurrentBright = FMath::Lerp(CurrentBright, InitialBright,
									DeltaTime * BurnDownSpeed);

		if ((CurrentBright - InitialBright) < .1f * (BurstBright - InitialBright))
		{
			// Reset
			CurrentBright = InitialBright;
			FlareComponent->SetLightBrightness(CurrentBright);
			Burning = false;
			Extinguishing = false;
		}
		else
		{
			FlareComponent->SetLightBrightness(CurrentBright);
		}
		float ActualBrightness = FlareComponent->ComputeLightBrightness();
		UE_LOG(LogTemp, Warning, TEXT("Burning with brightness %f"), ActualBrightness);
	}
}
