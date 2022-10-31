// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "../FGeneratorComponent.h"

class FTerrain : public FGeneratorComponent
{
public:
	using MapType = TMap<int32, TMap<int32, class UStaticMeshComponent*>>;

	FTerrain(FGeneratorComponent* RaceTracks);
	virtual ~FTerrain();

	MapType GetMap() const;
	FORCEINLINE int32 GetMaxRange() const { return TerrainMaxRange;  };

	class UStaticMeshComponent* GetNearestMeshRoad(class UStaticMeshComponent* TerrainMesh) const;

protected:
	void Generate() override;

private:
	double DownOffset = 100.0;
	int32 TerrainMaxRange;

	FGeneratorComponent* TrackComponent;
	MapType TerrainMap;
};