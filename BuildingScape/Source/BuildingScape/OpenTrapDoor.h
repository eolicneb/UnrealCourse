// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenTrapDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpenTrapDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenTrapDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SlideToTarget(float TargetPos, float DeltaTime);
	bool DidBonfireExplode() const;
	void PlayOpeningSound() const;
	void StopOpeningSound() const;

private:
	float ClosedPos = 0.f;
	float CurrentPos = 0.f;
	float OpenPos = 100.f;
	bool Open = false;

	UPROPERTY(EditAnywhere);
	ATriggerVolume* ActivationBonfire = nullptr;

	UPROPERTY(EditAnywhere);
	float SlideAmplitude = 100.f;

	UPROPERTY(EditAnywhere);
	float Slidespeed = .5f;

};
