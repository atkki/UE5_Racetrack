// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "../FGeneratorComponent.h"

class FTerrain : public FGeneratorComponent
{
public:
	using MapType = TMap<int32, TMap<int32, UStaticMeshComponent*>>;

	FTerrain(FGeneratorComponent* RaceTracks);
	virtual ~FTerrain();

	MapType GetMap();
	FORCEINLINE int32 GetMaxRange() { return TerrainMaxRange;  };

	UStaticMeshComponent* GetNearestMeshRoad(UStaticMeshComponent* TerrainMesh);

protected:
	void Generate() override;

private:
	double DownOffset = 100.0;
	int32 TerrainMaxRange;

	FGeneratorComponent* TrackComponent;
	MapType TerrainMap;
};