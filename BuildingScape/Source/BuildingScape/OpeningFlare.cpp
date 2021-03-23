// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningFlare.h"
#include "Components/AudioComponent.h"

#define BRIGHTMULT 100000.f

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
		// InitialBright = FlareComponent->ComputeLightBrightness();
		InitialPos = GetOwner()->GetActorLocation();
		CurrentPos = InitialPos;
		InitialBright = InitialPos.Z;
		UE_LOG(LogTemp, Warning, TEXT("Initial brightness of %s is %f"),
			*GetOwner()->GetName(), InitialBright);
	}

	if (!TriggerExplosive)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpeningFlare component but no TriggerExplosive!"),
			*GetOwner()->GetName());
	}
	else
	{
		Explosive = TriggerExplosive->FindComponentByClass<UIsExplosive>();
	}
	// Explode();

}


// Called every frame
void UOpeningFlare::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Burning && Explosive && Explosive->Exploded) { Explode(); }

	if (Burning)
	{
		UpdateBrightness(DeltaTime);
	}
	GetOwner()->SetActorLocation(CurrentPos
		 + FMath::VRand() * BurnAmplitud * (1 + CurrentBright/BurstBright));
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
	if (!Burning) { return; }
	if (!Extinguishing)
	{
		UE_LOG(LogTemp, Error, TEXT("BBOOOOOOMMMMMMMMM!!!!!! At bright %f"),
			BurstBright);
		CurrentBright = BurstBright;
		FlareComponent->SetLightBrightness(CurrentBright*CurrentBright*BRIGHTMULT);
		CurrentPos = InitialPos;
		CurrentPos.Z = CurrentBright;
		GetOwner()->SetActorLocation(CurrentPos);
		Extinguishing = true;
	}
	else
	{
		CurrentBright = FMath::Lerp(CurrentBright, InitialBright,
									DeltaTime * BurnDownSpeed);

		if ((CurrentBright - InitialBright) < .1f * (BurstBright - InitialBright))
		{
			// Reset
			// CurrentBright = InitialBright;
			FlareComponent->SetLightBrightness(CurrentBright*CurrentBright*BRIGHTMULT);
			// CurrentPos = GetOwner()->GetActorLocation();
			// CurrentPos.Z = CurrentBright;
			// GetOwner()->SetActorLocation(CurrentPos);
		}
		else
		{
			FlareComponent->SetLightBrightness(CurrentBright*CurrentBright*BRIGHTMULT);
			CurrentPos = InitialPos;
			CurrentPos.Z = CurrentBright;
			GetOwner()->SetActorLocation(CurrentPos);
		}

		UAudioComponent* FireComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
		if (FireComponent)
		{
			if(!FireComponent->IsPlaying()) { FireComponent->Play(); }

			FireComponent->VolumeMultiplier = CurrentBright/BurstBright;
			UE_LOG(LogTemp, Warning, TEXT("Current fire volume %f"),
			FireComponent->VolumeMultiplier);
		}

		// float ActualBrightness = FlareComponent->ComputeLightBrightness();
	}

}
