// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT(
					"%s has the OpenDoor component but no PressurePlate assigned!"),
			   *GetOwner()->GetName())
	}

	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MinOpenningMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(const float& DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * OpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	PlaySoundIfClosed();
}

void UOpenDoor::CloseDoor(const float& DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * CloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	PlaySoundIfClosed();
}

bool UOpenDoor::IsDoorClosed()
{
	return (FMath::Abs(CurrentYaw - InitialYaw) < ClosedAngleMargin);
}

void UOpenDoor::PlaySoundIfClosed()
{
	if (!AudioComponent) { return; }
	if (DoorClosed && !IsDoorClosed())
	{
		AudioComponent->Play();
		DoorClosed = false;
	}
	else if (!DoorClosed && IsDoorClosed())
	{
		AudioComponent->Play();
		DoorClosed = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	// Find All Overlapping Actors.
	if (!PressurePlate) { return 0.f; }

	TArray<UPrimitiveComponent*> OverlappingComponents;
	PressurePlate->GetOverlappingComponents(
		OUT OverlappingComponents);

	float TotalMassSum = 0.f;

	// Add up thier masses.
	for (int32 i=0; i < OverlappingComponents.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s weights %f"),
			*OverlappingComponents[i]->GetOwner()->GetName(),
			OverlappingComponents[i]->GetMass());
		TotalMassSum += OverlappingComponents[i]->GetMass();
	}

	UE_LOG(LogTemp, Error, TEXT("Total mass is %f"), TotalMassSum);

	return TotalMassSum;
}
