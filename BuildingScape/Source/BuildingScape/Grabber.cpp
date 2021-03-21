// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	GrabInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (GrabInput)
	{
		GrabInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		GrabInput->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has Grabber component but no Physics Handle component!"),
			*GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handel.
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetGrabbingPoint()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

	// Release the object we are holding.
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attach.
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetGrabbingPoint());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	GetGrabbingPoint(),
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FVector Location = GetViewPoint().VPLocation;
	FVector GrabbingPoint = GetGrabbingPoint();

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		Location,
		GrabbingPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object within reach is %s"), *ActorHit->GetName());
	}

	return Hit;
}

FPlayerViewPoint UGrabber::GetViewPoint() const
{
	// Get the player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,  // OUT is #defined to be nothing
		OUT PlayerViewPointRotation);

	FPlayerViewPoint ViewPoint = {PlayerViewPointLocation,
	                              PlayerViewPointRotation};
	return ViewPoint;
}

FVector UGrabber::GetGrabbingPoint() const
{
	// Get the player's viewpoint
	FPlayerViewPoint ViewPoint = GetViewPoint();

	FVector ReachVector = ViewPoint.VPRotation.Vector() * Reach;
	return ViewPoint.VPLocation + ReachVector;
}
