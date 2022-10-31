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

FTerrain::MapType FTerrain::GetMap() const
{
	return this->TerrainMap;
}

UStaticMeshComponent* FTerrain::GetNearestMeshRoad(UStaticMeshComponent* TerrainMesh) const
{
	return static_cast<UStaticMeshComponent*>(TerrainMesh->GetAttachParent());
}

void FTerrain::Generate()
{
	if (GetGenerator()->TerrainMesh == nullptr) return;

	const auto Tracks = TrackComponent->GetMeshes();

	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);
	this->DownOffset = TerrainMeshSize.Z * 0.5;

	const FVector StartPosition { GetGenerator()->GetActorLocation() };
	const FVector FirstTrack = Tracks[0]->GetRelativeLocation();
	const FVector LastTrack = Tracks.Top()->GetRelativeLocation();
	const double Distance = FVector::Dist(FirstTrack, LastTrack);

	// create tile map
	const int32 TerrainRange = this->GetMaxRange();
	for (int32 X = 0; X < TerrainRange; X += TerrainMeshSize.X)
	{
		TerrainMap.Add(X, TMap<int32, UStaticMeshComponent*>{});

		for (int32 Y = -TerrainRange; Y < TerrainRange; Y += TerrainMeshSize.Y)
		{
			const FVector TerrainVector(X, Y, FirstTrack.Z); // direct initialization to skip useless conversion code

			UStaticMeshComponent* NearestTrack = Tracks[0];
			for (auto* Track : Tracks)
			{
				if (FVector::Dist(Track->GetRelativeLocation(), TerrainVector) < FVector::Dist(NearestTrack->GetRelativeLocation(), TerrainVector))
					NearestTrack = Track;
			}

			// create terrain mesh only if nearest track isn't too far
			if (FVector::Dist(NearestTrack->GetRelativeLocation(), TerrainVector) < TerrainMeshSize.Y * 5)
			{
				FTransform TerrainTransform{
					FRotator{0, 0, 0},
					FVector(X, Y, -this->DownOffset),
					FVector{ 1.0, 1.0, 1.0 }
				};

				UStaticMeshComponent* Mesh = this->AddMesh(GetGenerator()->TerrainMesh, TerrainTransform);
				Mesh->AttachToComponent(NearestTrack, FAttachmentTransformRules::KeepWorldTransform);
				TerrainMap[X].Add(Y, Mesh);
			}
			else 
			{
				TerrainMap[X].Add(Y, nullptr);
			}
		}
	}
}
