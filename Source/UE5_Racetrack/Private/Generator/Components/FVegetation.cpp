#include "Generator/Components/FVegetation.h"

#include "Generator/AGenerator.h"
#include "Generator/Components/FTerrain.h"

FVegetation::FVegetation(FGeneratorComponent* Terrain)
	: FGeneratorComponent{ Terrain->GetGenerator() }, TerrainComponent { Terrain }
{
	this->Generate();
}

FVegetation::~FVegetation()
{
}

void FVegetation::Generate()
{
	if (GetGenerator()->TreeMesh == nullptr) return;
	if (GetGenerator()->BushMesh == nullptr) return;

	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);
	const FVector BushMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->BushMesh);

	const FTerrain::MapType TerrainMap = static_cast<FTerrain*>(TerrainComponent)->GetMap();
	const int32 TerrainRange = static_cast<FTerrain*>(TerrainComponent)->GetMaxRange();

	for (int32 x = 0; x < TerrainRange; x += TerrainMeshSize.X)
	{
		for (int32 y = -TerrainRange; y < TerrainRange; y += TerrainMeshSize.Y)
		{
			UStaticMeshComponent* TerrainMesh = TerrainMap[x][y];
			if (TerrainMesh != nullptr)
			{
				UStaticMeshComponent* RoadMesh = static_cast<FTerrain*>(TerrainComponent)->GetNearestMeshRoad(TerrainMesh);
				if (RoadMesh != nullptr 
					&& FVector::Dist(RoadMesh->GetComponentLocation(), TerrainMesh->GetComponentLocation()) > TerrainMeshSize.X * 6
					&& GetGenerator()->VegetationDensity + FMath::RandRange(0, 100) > 100)
				{
					UStaticMesh* Mesh;
					FVector Position;
					double RandomScale;
					if (FMath::RandRange(1, 10) >= 6 || RoadMesh->GetStaticMesh() == GetGenerator()->LeftCornerTrackMesh)
					{
						Position = FVector(x, y, 0);
						Mesh = GetGenerator()->TreeMesh;
						RandomScale = FMath::RandRange(0.8, 2.0);
					}
					else
					{
						Position = FVector(x, y, -FMath::RandRange(BushMeshSize.Z / 4, BushMeshSize.Z));
						Mesh = GetGenerator()->BushMesh;
						RandomScale = FMath::RandRange(0.7, 1.3);
					}
	
					FTransform MeshTransform{
						FRotator{0, 0, 0},
						Position,
						FVector{RandomScale, RandomScale, RandomScale}
					};

					this->AddMesh(Mesh, MeshTransform)->AttachToComponent(TerrainMesh, FAttachmentTransformRules::KeepWorldTransform);
				}
			}
		}
	}
}