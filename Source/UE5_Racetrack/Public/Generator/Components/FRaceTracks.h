#pragma once

#include "Containers/Array.h"
#include "../FGeneratorComponent.h"

class FRaceTracks : public FGeneratorComponent
{
public:
	FRaceTracks(class AGenerator* CurrentGenerator);
	virtual ~FRaceTracks() = default;
protected:
	virtual void Generate() override;
};