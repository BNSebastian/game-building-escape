// Copyright of Bucovala Sebastian 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	void ShowReach(FVector PlayerLocation, FVector LineTraceEnd) const;
	
	//return the first actor withn reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// return the player location
	FVector GetPlayerLocation() const;

	// return the line trace end
	FVector GetLineTraceEnd() const;

private:
	UPROPERTY(EditAnywhere)float reach = 160.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
