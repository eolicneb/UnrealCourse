// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
	    FActorComponentTickFunction* ThisTickFunction
	) override;
	void OpenDoor(const float& DeltaTime);
	void CloseDoor(const float& DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	bool IsDoorClosed();
	void PlaySoundIfClosed();

private:

	float DoorLastOpened = 0.f;
	float CurrentYaw;
	float InitialYaw;
	bool DoorClosed = true;

	UPROPERTY(EditAnywhere);
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere);
	float OpenAngle = -90.f;

	UPROPERTY(EditAnywhere);
	float ClosedAngleMargin = -2.f;

	UPROPERTY(EditAnywhere);
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere);
	float OpenSpeed = .5f;

	UPROPERTY(EditAnywhere);
	float CloseSpeed = 2.f;

	UPROPERTY(EditAnywhere);
	float MinOpenningMass = 50.f;

	UPROPERTY(EditAnywhere);
	UAudioComponent* AudioComponent = nullptr;
};
