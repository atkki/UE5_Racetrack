#pragma once

#include "../FGeneratorComponent.h"

class FVegetation : public FGeneratorComponent
{
public:
	FVegetation(FGeneratorComponent* Terrain);
	virtual ~FVegetation() = default;

protected:
	void Generate() override;

private:
	FGeneratorComponent* TerrainComponent;
};