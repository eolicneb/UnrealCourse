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

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has Grabber component but no Physics Handle component!"),
			*GetOwner()->GetName())
	}
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

void UGrabber::Grab()
{
	// Get the player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,  // OUT is #defined to be nothing
		OUT PlayerViewPointRotation);

	// Draw line from player showing the reach

	FVector ReachVector = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + ReachVector;

	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handel.
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
	// TODO attach the physics handle.
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

	// Move the object we are holding.
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,  // OUT is #defined to be nothing
		OUT PlayerViewPointRotation);

	// Draw line from player showing the reach

	FVector ReachVector = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + ReachVector;

	// If the physics handle is attach.
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get the player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,  // OUT is #defined to be nothing
		OUT PlayerViewPointRotation);

	// Draw line from player showing the reach

	FVector ReachVector = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + ReachVector;

	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );

	FHitResult Hit;
	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
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
