// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"

struct FPlayerViewPoint
{
	FVector VPLocation;
	FRotator VPRotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;


	UPROPERTY(EditAnywhere);
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY(EditAnywhere);
	UInputComponent* GrabInput = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach() const;
	FVector GetGrabbingPoint() const;
	FPlayerViewPoint GetViewPoint() const;
};
