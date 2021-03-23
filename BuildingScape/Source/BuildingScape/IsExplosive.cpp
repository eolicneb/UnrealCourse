// Fill out your copyright notice in the Description page of Project Settings.


#include "IsExplosive.h"
#include "engine/World.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UIsExplosive::UIsExplosive()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UIsExplosive::BeginPlay()
{
	Super::BeginPlay();

	if (!ActivationBonfire)
	{
		UE_LOG(LogTemp, Error, TEXT(
					"%s has the OpenTrapDoor component but no ActivationBonfire assigned!"),
			   *GetOwner()->GetName())
	}

}


// Called every frame
void UIsExplosive::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetBurnt();
}

void UIsExplosive::GetBurnt()
{
	if (!AmIInBonfire())
	{
		IsHeatting = false;
		Exploded = false;
	}
	else if (!Exploded && !IsHeatting)
	{
		IsHeatting = true;
		BurnTimeStart = GetWorld()->GetTimeSeconds();
	}
	else if (!Exploded && GetWorld()->GetTimeSeconds() - BurnTimeStart > ExplosionDelay)
	{
		UE_LOG(LogTemp, Error, TEXT("%s went BBBOOOOMMM!!!!!"), *GetOwner()->GetName());
		Exploded = true;
		GetOwner()->FindComponentByClass<UAudioComponent>()->Play();
	}
}

bool UIsExplosive::AmIInBonfire() const
{
	if (!ActivationBonfire) { return false; }

	TArray<AActor*> OverlappingActors;
	ActivationBonfire->GetOverlappingActors(
		OUT OverlappingActors);

	for (AActor* Component : OverlappingActors)
	{
		if (Component == GetOwner())
		{
			return true;
		}
	}

	return false;
}
