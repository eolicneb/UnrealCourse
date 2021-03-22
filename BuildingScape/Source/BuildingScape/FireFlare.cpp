// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFlare.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UFireFlare::UFireFlare()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFireFlare::BeginPlay()
{
	Super::BeginPlay();

	InitialPos = GetOwner()->GetActorLocation();
}


// Called every frame
void UFireFlare::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = InitialPos + FMath::VRand() * BurnAmplitud;
	GetOwner()->SetActorLocation(CurrentLocation);
}

