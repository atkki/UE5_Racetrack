#include "FUtils.h"
#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"

FVector FUtils::GetOffsetLocation(UStaticMeshComponent* Mesh, FVector Offset, bool WorldSpace)
{
	if (Mesh == nullptr) return FVector{ 0, 0, 0 };
	
	FVector Position;
	FVector Scale{ 1, 1, 1 };
	if (WorldSpace)
	{
		Position = Mesh->GetComponentLocation();
		Scale = Mesh->GetComponentScale();
	}
	else
	{
		Position = Mesh->GetRelativeLocation();
	}

	Position += (Mesh->GetForwardVector() * Offset.X) * Scale.X;
	Position += (Mesh->GetRightVector() * Offset.Y) * Scale.Y;
	Position += (Mesh->GetUpVector() * Offset.Z) * Scale.Z;
	return Position;
}