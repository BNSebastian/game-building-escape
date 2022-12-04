// Copyright of Bucovala Sebastian 2022

#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Transform.h"

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
	TargetYaw += InitialYaw;

	FindPressurePlate();

	ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn(); // pointer to the PC
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && TotalMass() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (PressurePlate && GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorClosedDelay)
	{
		CloseDoor(DeltaTime);
    }
}


void UOpenDoor::OpenDoor(float DeltaTime)
{
	FString object_name = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("The object: %s has an yaw of: %f"), *object_name, CurrentYaw);
	
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, OpenSpeed); // constant interpolation of current yaw
	//open_door.Yaw = FMath::FInterpTo(current_yaw, target_yaw, DeltaTime, 2); // constant interpolation of current yaw
	//open_door.Yaw = FMath::Lerp(current_yaw, target_yaw, 0.02f); // exponential interpolation of current yaw 

	FRotator door_rotation = GetOwner()->GetActorRotation(); // create a rotator with the target rotation
	door_rotation.Yaw = CurrentYaw; // set rotator yaw to the current yaw
	GetOwner()->SetActorRotation(door_rotation); // update actor rotation

	CloseDoorSound = false;

	if (!AudioComponent) { return; }

	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);

	FRotator door_rotation = GetOwner()->GetActorRotation(); // create a rotator with the target rotation
	door_rotation.Yaw = CurrentYaw; // set rotator yaw to the current yaw
	GetOwner()->SetActorRotation(door_rotation); // update actor rotation

	OpenDoorSound = false;

	if (!AudioComponent) { return; }

	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMass() const
{
	float TotalMass = 0.f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressureplate!"), *Actor->GetName());
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{

}
