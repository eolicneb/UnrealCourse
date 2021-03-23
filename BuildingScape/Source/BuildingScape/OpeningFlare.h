// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PointLightComponent.h"
#include "IsExplosive.h"
#include "OpeningFlare.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpeningFlare : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpeningFlare();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Explode();
	void UpdateBrightness(float DeltaTime);

private:
	FVector InitialPos = {0.f, 0.f, 0.f};
	FVector CurrentPos = {0.f, 0.f, 0.f};
	float InitialBright = 0.f;
	float CurrentBright = 0.f;
	float IgnitionTime = 0.f;
	bool Burning = false;
	bool Extinguishing = false;

	ULightComponent* FlareComponent = nullptr;
	UIsExplosive* Explosive = nullptr;

	UPROPERTY(EditAnywhere);
	float BurstBright = 10.f;

	UPROPERTY(EditAnywhere);
	float BurnDownSpeed = .1f;

	UPROPERTY(EditAnywhere);
	float BurnAmplitud = 10.f;

	UPROPERTY(EditAnywhere);
	AActor* TriggerExplosive = nullptr;
};
