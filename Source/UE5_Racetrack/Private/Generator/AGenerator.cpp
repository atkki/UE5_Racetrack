// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator/AGenerator.h"
#include "Generator/Components/FRaceTracks.h"
#include "Generator/Components/FTerrain.h"
#include "Generator/Components/FVegetation.h"
#include "Generator/Components/FDecorations.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

AGenerator::AGenerator()
{
	if (WITH_EDITOR)
	{
		PrimaryActorTick.bStartWithTickEnabled = true;
		PrimaryActorTick.bCanEverTick = true;
	}

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
}

// Tick enabled only in editor
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RaceTracks.IsValid())
	{
		auto LastTrack = RaceTracks->GetLastMesh();
		DrawDebugPoint(
			GetWorld(),
			LastTrack->GetComponentLocation(),
			50,
			FColor(255, 0, 0)
		);

		DrawDebugLine(
			GetWorld(),
			LastTrack->GetComponentLocation(),
			LastTrack->GetComponentLocation() + (LastTrack->GetForwardVector() * 5000),
			FColor(0, 255, 0),
			false,
			-1,
			0,
			10
		);
	}
}

bool AGenerator::ShouldTickIfViewportsOnly() const
{
	return true;
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the actor changes in-game / in-editor
void AGenerator::OnConstruction(const FTransform& Transform)
{
	this->Cleanup();
	this->Generate();
}

void AGenerator::Generate()
{
	// Generate main race track
	RaceTracks = MakeShared<FRaceTracks>(this);

	// Fill terrain based on generated race track
	Terrain = MakeShared<FTerrain>(RaceTracks.Get());

	// Fill trees & bushes
	Vegetation = MakeShared<FVegetation>(Terrain.Get());

	// Fill additional decorations near race tracks
	Decorations = MakeShared<FDecorations>(RaceTracks.Get());
}

UStaticMeshComponent* AGenerator::NewMesh(UStaticMesh* Mesh, const FTransform& MeshTransform)
{
	UStaticMeshComponent* Obj = NewObject<UStaticMeshComponent>(this);
	Obj->RegisterComponent();
	Obj->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Obj->SetStaticMesh(Mesh);
	Obj->SetRelativeTransform(MeshTransform);
	return Obj;
}

UInstancedStaticMeshComponent* AGenerator::NewMeshInstance(UStaticMesh* Mesh, const FTransform& MeshTransform)
{
	UInstancedStaticMeshComponent* Obj = NewObject<UInstancedStaticMeshComponent>(this);
	Obj->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Obj->RegisterComponent();
	Obj->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Obj->SetStaticMesh(Mesh);
	Obj->AddInstance(MeshTransform);
	return Obj;
}

void AGenerator::Cleanup()
{
	while (this->GetComponentByClass(UInstancedStaticMeshComponent::StaticClass()) != nullptr) {
		UActorComponent* ComponentToClean = this->GetComponentByClass(UInstancedStaticMeshComponent::StaticClass());
		ComponentToClean->DestroyComponent();
	}

	while (this->GetComponentByClass(UStaticMeshComponent::StaticClass()) != nullptr) {
		UActorComponent* ComponentToClean = this->GetComponentByClass(UStaticMeshComponent::StaticClass());
		ComponentToClean->DestroyComponent();
	}

	RaceTracks.Reset();
	Terrain.Reset();
	Vegetation.Reset();
	Decorations.Reset();

	if (GEngine)
		GEngine->ForceGarbageCollection(true);
}