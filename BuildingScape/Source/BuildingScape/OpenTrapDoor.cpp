// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenTrapDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "IsExplosive.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenTrapDoor::UOpenTrapDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenTrapDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedPos = GetOwner()->GetActorLocation()[0];
	OpenPos = SlideAmplitude + ClosedPos;

	if (!ActivationBonfire)
	{
		UE_LOG(LogTemp, Error, TEXT(
					"%s has the OpenTrapDoor component but no ActivationBonfire assigned!"),
			   *GetOwner()->GetName())
	}
}


// Called every frame
void UOpenTrapDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DidBonfireExplode())
	{
		SlideToTarget(OpenPos, DeltaTime);
		PlayOpeningSound();
		Open = true;
	}
}

void UOpenTrapDoor::SlideToTarget(float TargetPos, float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	CurrentPos = CurrentLocation[0];
	float UpdatePos = FMath::Lerp(CurrentPos, OpenPos, DeltaTime * Slidespeed);
	CurrentLocation[0] = UpdatePos;
	GetOwner()->SetActorLocation(CurrentLocation);
	if (FMath::Abs(OpenPos-CurrentPos) < 0.1f * FMath::Abs(SlideAmplitude))
	{
		StopOpeningSound();
	}
}

bool UOpenTrapDoor::DidBonfireExplode() const
{
	if (!ActivationBonfire) { return 0.f; }

	// Find All Overlapping Actors.
	TArray<AActor*> OverlappingActors;
	ActivationBonfire->GetOverlappingActors(
		OUT OverlappingActors);

	// Add up thier masses.
	for (AActor* Actor : OverlappingActors)
	{
		UIsExplosive* Explosive = Actor->FindComponentByClass<UIsExplosive>();
		if (Explosive)
		{
			return Explosive->Exploded;
		}
	}

	return false;
}

void UOpenTrapDoor::PlayOpeningSound() const
{
	UAudioComponent* StoneSoundComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (StoneSoundComponent && !Open)
	{
		StoneSoundComponent->Play();
	}
}

void UOpenTrapDoor::StopOpeningSound() const
{
	UAudioComponent* StoneSoundComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (StoneSoundComponent)
	{
		StoneSoundComponent->Stop();
	}
}
