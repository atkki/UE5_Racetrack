#pragma once

#include "../FGeneratorComponent.h"

class FDecorations : public FGeneratorComponent
{
public:
	FDecorations(FGeneratorComponent* Tracks);
	virtual ~FDecorations() = default;

protected:
	void Generate() override;

private:
	FGeneratorComponent* TracksComponent;
};