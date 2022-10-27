#pragma once
#include "Containers/Array.h"
#include "Math/TransformNonVectorized.h"

class FGeneratorComponent
{
public:
	virtual TArray<class UStaticMeshComponent*>& GetMeshes();
	class UStaticMeshComponent* GetLastMesh() const;
	class AGenerator* GetGenerator() const;

protected:
	FGeneratorComponent(class AGenerator* Gen);
	~FGeneratorComponent();

	virtual void Generate() = 0;
	virtual UStaticMeshComponent* AddMesh(class UStaticMesh* Mesh, FTransform& Transform);

private: 
	class AGenerator* Generator;
	TArray<class UStaticMeshComponent*> Meshes;
	class UStaticMeshComponent* Last;
};