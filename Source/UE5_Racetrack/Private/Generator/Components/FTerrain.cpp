// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator/Components/FTerrain.h"
#include "Generator/AGenerator.h"
#include "FUtils.h"

#include "Math/TransformNonVectorized.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

FTerrain::FTerrain(FGeneratorComponent* RaceTracks)
	:	FGeneratorComponent{ RaceTracks->GetGenerator() },
		TrackComponent{ RaceTracks },
		TerrainMap{ FTerrain::MapType { } }
{
	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);
	TerrainMaxRange = 50 + GetGenerator()->TrackLength * TerrainMeshSize.X;
	TerrainMap.Reserve(TerrainMaxRange * 2);

	this->Generate();
}

FTerrain::~FTerrain()
{
	this->TerrainMap.Empty();
}

FTerrain::MapType FTerrain::GetMap()
{
	return this->TerrainMap;
}

UStaticMeshComponent* FTerrain::GetNearestMeshRoad(UStaticMeshComponent* TerrainMesh)
{
	return static_cast<UStaticMeshComponent*>(TerrainMesh->GetAttachParent());
}

void FTerrain::Generate()
{
	if (GetGenerator()->TerrainMesh == nullptr) return;

	auto Tracks = TrackComponent->GetMeshes();

	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);
	this->DownOffset = TerrainMeshSize.Z * 0.5;

	const FVector StartPosition { GetGenerator()->GetActorLocation() };
	const FVector FirstTrack = Tracks[0]->GetRelativeLocation();
	const FVector LastTrack = Tracks.Top()->GetRelativeLocation();
	double Distance = FVector::Dist(FirstTrack, LastTrack);

	int32 TerrainRange = this->GetMaxRange();
	for (int32 x = 0; x < TerrainRange; x += TerrainMeshSize.X)
	{
		TerrainMap.Add(x, TMap<int32, UStaticMeshComponent*>{});

		for (int32 y = -TerrainRange; y < TerrainRange; y += TerrainMeshSize.Y)
		{
			FVector TerrainVector(x, y, FirstTrack.Z); // direct initialization to skip useless conversion code

			UStaticMeshComponent* NearestTrack = Tracks[0];
			for (auto* Track : Tracks)
			{
				if (FVector::Dist(Track->GetRelativeLocation(), TerrainVector) < FVector::Dist(NearestTrack->GetRelativeLocation(), TerrainVector))
					NearestTrack = Track;
			}

			if (FVector::Dist(NearestTrack->GetRelativeLocation(), TerrainVector) < TerrainMeshSize.Y * 5)
			{
				FTransform TerrainTransform{
					FRotator{0, 0, 0}, // Rotation
					FVector(x, y, -this->DownOffset), // Position
					FVector{ 1.0, 1.0, 1.0 } // Scale
				};

				UStaticMeshComponent* Mesh = this->AddMesh(GetGenerator()->TerrainMesh, TerrainTransform);
				Mesh->AttachToComponent(NearestTrack, FAttachmentTransformRules::KeepWorldTransform);
				TerrainMap[x].Add(y, Mesh);
			}
			else 
			{
				TerrainMap[x].Add(y, nullptr);
			}
		}
	}
}
