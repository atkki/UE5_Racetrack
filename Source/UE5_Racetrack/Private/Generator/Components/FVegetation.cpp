#include "Generator/Components/FVegetation.h"

#include "Generator/AGenerator.h"
#include "Generator/Components/FTerrain.h"

FVegetation::FVegetation(FGeneratorComponent* Terrain)
	: FGeneratorComponent{ Terrain->GetGenerator() }, TerrainComponent { Terrain }
{
	Generate();
}

void FVegetation::Generate()
{
	if (GetGenerator()->TreeMesh == nullptr) return;
	if (GetGenerator()->BushMesh == nullptr) return;
	if (GetGenerator()->RockMesh == nullptr) return;

	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);
	const FVector BushMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->BushMesh);

	const FTerrain* TerrainPtr = static_cast<FTerrain*>(TerrainComponent);
	const FTerrain::MapType TerrainMap = TerrainPtr->GetMap();
	const int32 TerrainRange = TerrainPtr->GetMaxRange();

	// on every tile of terrain
	for (int32 X = 0; X < TerrainRange; X += TerrainMeshSize.X)
	{
		for (int32 Y = -TerrainRange; Y < TerrainRange; Y += TerrainMeshSize.Y)
		{
			UStaticMeshComponent* TerrainMesh = TerrainMap[X][Y];
			if (TerrainMesh != nullptr)
			{
				// create vegetation, if nearest road is far enough to not intersect
				UStaticMeshComponent* RoadMesh = TerrainPtr->GetNearestMeshRoad(TerrainMesh);
				if (RoadMesh != nullptr 
					&& FVector::Dist(RoadMesh->GetComponentLocation(), TerrainMesh->GetComponentLocation()) > TerrainMeshSize.X * 6
					&& GetGenerator()->VegetationDensity + FMath::RandRange(0, 100) > 100)
				{
					UStaticMesh* Mesh;
					FVector Position;
					double RandomScale;
					if (FMath::RandRange(1, 10) >= 4 || RoadMesh->GetStaticMesh() == GetGenerator()->LeftCornerTrackMesh)
					{
						Position = FVector(X, Y, 0);
						Mesh = GetGenerator()->TreeMesh;
						RandomScale = FMath::RandRange(0.8, 2.0);
					}
					else
					{
						Position = FVector(X, Y, -FMath::RandRange(BushMeshSize.Z * 0.25, BushMeshSize.Z * 0.5));
						Mesh = FMath::RandBool() ? GetGenerator()->BushMesh : GetGenerator()->RockMesh;
						RandomScale = FMath::RandRange(0.7, 1.3);
					}
	
					FTransform MeshTransform{
						FRotator(0, FMath::RandRange(0, 360), 0),
						Position,
						FVector{RandomScale, RandomScale, RandomScale}
					};

					AddMesh(Mesh, MeshTransform)->AttachToComponent(TerrainMesh, FAttachmentTransformRules::KeepWorldTransform);
				}
			}
		}
	}
}