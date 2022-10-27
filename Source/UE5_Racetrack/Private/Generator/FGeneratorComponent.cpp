#include "Generator/FGeneratorComponent.h"
#include "Generator/AGenerator.h"

FGeneratorComponent::FGeneratorComponent(AGenerator* Gen)
	: Generator { Gen }, Meshes{ TArray<UStaticMeshComponent*>{} }, Last{ nullptr }
{
	Meshes.Reserve(Generator->TrackLength);
}

FGeneratorComponent::~FGeneratorComponent()
{
	Meshes.Empty();
}

UStaticMeshComponent* FGeneratorComponent::AddMesh(UStaticMesh* Mesh, FTransform& Transform)
{
	UStaticMeshComponent* MeshComponent = Generator->NewMesh(Mesh, Transform);
	Last = MeshComponent;
	Meshes.Add(MeshComponent);

	return MeshComponent;
}

TArray<UStaticMeshComponent*>& FGeneratorComponent::GetMeshes()
{
	return Meshes;
}

UStaticMeshComponent* FGeneratorComponent::GetLastMesh() const
{
	return Last;
}

AGenerator* FGeneratorComponent::GetGenerator() const
{
	return Generator;
}
