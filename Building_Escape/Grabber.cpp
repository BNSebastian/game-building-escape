// Copyright of Bucovala Sebastian 2022

#include "Grabber.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. 
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()     
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

// check for the physics handle component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NO physics handle found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("input component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); // bind function to input
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release); // bind function to input
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO input component found on % s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// if we hit something attach the physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
			return;

		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetLineTraceEnd()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("pressing the release button"));

	if (!PhysicsHandle)
		return;
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
		return;
	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object we are holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

// ray-cast to a certain distance (reach)
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayerLocation(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	
	return Hit;
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerLocation,
		PlayerRotation
	);
	return PlayerLocation;
}

FVector UGrabber::GetLineTraceEnd() const
{
	// get player viewpoint
	FVector PlayerLocation;
	FRotator PlayerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerLocation,
		PlayerRotation
	);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * reach;
	return LineTraceEnd;
}

// draw a line from the player showing the reach
void UGrabber::ShowReach(FVector PlayerLocation, FVector LineTraceEnd) const
{
	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(0, 255, 0), // line color
		false,
		0.f,
		0,
		5 // line width in pixels
	);
}