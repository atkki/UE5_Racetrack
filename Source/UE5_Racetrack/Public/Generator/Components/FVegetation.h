#pragma once

#include "../FGeneratorComponent.h"

class FVegetation : public FGeneratorComponent
{
public:
	FVegetation(FGeneratorComponent* Terrain);
	virtual ~FVegetation();

protected:
	void Generate() override;

private:
	FGeneratorComponent* TerrainComponent;
};