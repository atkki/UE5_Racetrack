// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AVehicle.generated.h"

UCLASS()
class UE5_RACETRACK_API AVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	float GetRPM() const;
	float GetEngineTorque() const;
	float GetEngineMaxTorque() const;

protected:
	virtual void BeginPlay() override;
	void InputHandbrake();
	void InputThrottle(float Value);
	void InputBrake(float Value);
	void InputSteering(float Value);

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Audio)
	class UAudioComponent* EngineAudio;
};
