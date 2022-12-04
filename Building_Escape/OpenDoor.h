// Copyright of Bucovala Sebastian 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMass() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float InitialYaw;
	float CurrentYaw;
	UPROPERTY(EditAnywhere) 
	float TargetYaw = 90.f; // expose target_yaw to the editor

	UPROPERTY(EditAnywhere) 
	int32 OpenSpeed = 100; // door opening speed
	UPROPERTY(EditAnywhere) 
	int32 CloseSpeed = 200; // door closing speed

	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere) 
	float DoorClosedDelay = 0.3f;

	AActor* ActorThatOpensDoor; // trigger actor
	UPROPERTY(EditAnywhere) 
	ATriggerVolume *PressurePlate; // trigger volume

	UPROPERTY(EditAnywhere) 
	float MassToOpenDoor = 50.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
