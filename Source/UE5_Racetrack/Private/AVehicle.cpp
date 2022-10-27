// Fill out your copyright notice in the Description page of Project Settings.


#include "AVehicle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ChaosVehicleMovementComponent.h"

AVehicle::AVehicle()
	: CameraBoom{ CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")) },
	FollowCamera{ CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")) },
	EngineAudio{ CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio")) }
{
	CameraBoom->SetupAttachment(GetMesh());

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	EngineAudio->SetupAttachment(GetMesh());
	EngineAudio->Play();

	/* make movement more arcade */
	FVehicleTorqueControlConfig& TorqueControl = GetVehicleMovementComponent()->TorqueControl;
	TorqueControl.Enabled = true;
	TorqueControl.YawTorqueScaling = -0.5f;
	TorqueControl.YawFromSteering = 0.25f;
	TorqueControl.YawFromRollTorqueScaling = -0.75f;
	TorqueControl.PitchTorqueScaling = -5.0f;
	TorqueControl.RollTorqueScaling = -5.0f;
	TorqueControl.RollFromSteering = -0.5f;
	TorqueControl.RotationDamping = 0.5f;
}
void AVehicle::Tick(float DeltaTime)
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Gear: %i"), this->GetVehicleMovementComponent()->GetCurrentGear()));
	EngineAudio->SetFloatParameter(FName("RPM"), this->GetRPM());
	EngineAudio->SetFloatParameter(FName("Throttle"), this->GetEngineTorque() / this->GetEngineMaxTorque());
	EngineAudio->SetFloatParameter(FName("Speed"), this->GetVehicleMovementComponent()->GetForwardSpeed());

	/* on jumps rotates too much, adjust that */
	if (this->GetVehicleMovementComponent()->IsMovingOnGround() == false)
	{
		FRotator Rotation = this->GetActorRotation();
		if (Rotation.Pitch < -15)
		{
			this->GetMesh()->AddTorqueInDegrees(FVector(0, -200.0, 0), NAME_None, true);
		}
		else if (Rotation.Pitch > 15)
		{
			this->GetMesh()->AddTorqueInDegrees(FVector(0, 200.0, 0), NAME_None, true);
		}
	}
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Handbrake", IE_Repeat, this, &AVehicle::Handbrake);

	PlayerInputComponent->BindAxis("Throttle", this, &AVehicle::Throttle);
	PlayerInputComponent->BindAxis("Brake", this, &AVehicle::Brake);
	PlayerInputComponent->BindAxis("Steering", this, &AVehicle::Steering);
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicle::Handbrake()
{
	this->GetVehicleMovementComponent()->SetHandbrakeInput(!GetVehicleMovementComponent()->GetHandbrakeInput());
}

void AVehicle::Throttle(float Value)
{
	this->GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void AVehicle::Brake(float Value)
{
	this->GetVehicleMovementComponent()->SetBrakeInput(Value);
}

void AVehicle::Steering(float Value)
{
	this->GetVehicleMovementComponent()->SetSteeringInput(Value);
}

float AVehicle::GetRPM() const
{
	auto Component = Cast<UChaosWheeledVehicleMovementComponent>(this->GetVehicleMovementComponent());
	if (Component != nullptr)
		return Component->GetEngineRotationSpeed();

	return 0.0f;
}

float AVehicle::GetEngineTorque() const
{
	TUniquePtr<FPhysicsVehicleOutput>& PhysicsOutput = this->GetVehicleMovementComponent()->PhysicsVehicleOutput();
	if (PhysicsOutput.IsValid())
		return PhysicsOutput.Get()->EngineTorque;

	return 0.0f;
}

float AVehicle::GetEngineMaxTorque() const
{
	auto Component = Cast<UChaosWheeledVehicleMovementComponent>(this->GetVehicleMovementComponent());
	if (Component != nullptr)
		return Component->EngineSetup.MaxTorque;

	return 0.0f;
}
