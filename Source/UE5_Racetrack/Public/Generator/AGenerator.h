// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FUtils.h"

#include "AGenerator.generated.h"

UCLASS()
class UE5_RACETRACK_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AGenerator();
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Race Track)")
	class UStaticMesh* ForwardTrackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Race Track)")
	class UStaticMesh* LeftCornerTrackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Race Track)")
	class UStaticMesh* JumpTrackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Terrain)")
	class UStaticMesh* TerrainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Vegetation)")
	class UStaticMesh* TreeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Vegetation)")
	class UStaticMesh* BushMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Vegetation)")
	class UStaticMesh* RockMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Decorations)")
	class UStaticMesh* BleachersMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Decorations)")
	class UStaticMesh* WatchtowerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Decorations)")
	class UStaticMesh* CheckpointMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets (Decorations)")
	class UStaticMesh* FinishMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 TrackLength = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 TurnSpace = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 VegetationDensity = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 DecorationsDensity = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	class USceneComponent* SceneRoot;

	class UStaticMeshComponent* NewMesh(class UStaticMesh* Mesh, const FTransform& MeshTransform);
	class UInstancedStaticMeshComponent* NewMeshInstance(class UStaticMesh* Mesh, const FTransform& MeshTransform);

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private: 
	void Generate();
	void Cleanup();

	TSharedPtr<class FGeneratorComponent> RaceTracks;
	TSharedPtr<class FGeneratorComponent> Terrain;
	TSharedPtr<class FGeneratorComponent> Vegetation;
	TSharedPtr<class FGeneratorComponent> Decorations;

};
