#pragma once
#include "Math/Vector.h"

class FUtils 
{
public:
	static FVector GetOffsetLocation(class UStaticMeshComponent* Mesh, FVector Offset, bool WorldSpace = false);
	FORCEINLINE static FVector GetStaticMeshSize(class UStaticMeshComponent* Mesh) { return Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize(); };
	FORCEINLINE static FVector GetStaticMeshSize(class UStaticMesh* Mesh) { return Mesh->GetBounds().GetBox().GetSize(); };
};