#include "Generator/Components/FDecorations.h"

#include "Generator/AGenerator.h"
#include "Generator/Components/FRaceTracks.h"

FDecorations::FDecorations(FGeneratorComponent* Tracks)
	: FGeneratorComponent{ Tracks->GetGenerator() }, TracksComponent{ Tracks }
{
	this->Generate();
}

void FDecorations::Generate()
{
	if (GetGenerator()->BleachersMesh == nullptr) return;
	if (GetGenerator()->WatchtowerMesh == nullptr) return;
	if (GetGenerator()->CheckpointMesh == nullptr) return;
	if (GetGenerator()->FinishMesh == nullptr) return;

	FVector TrackMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->ForwardTrackMesh);
	FVector CornerMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->LeftCornerTrackMesh);

	int8 BleachersWait = 0;
	int8 LastCheckpoint = 0;

	auto TrackMeshes = TracksComponent->GetMeshes();
	for (int32 Index = 0; Index != TrackMeshes.Num(); ++Index)
	{
		auto Track = TrackMeshes[Index];
		if (GetGenerator()->DecorationsDensity + FMath::RandRange(0, 100) > 100)
		{
			--BleachersWait;

			if (Track->GetStaticMesh() == GetGenerator()->ForwardTrackMesh && FMath::RandRange(1, 2) == 1 && BleachersWait < 0)
			{
				// left bleachers
				FRotator Rotation = Track->GetRelativeRotation();
				FTransform MeshTransform{
					FRotator{Rotation.Pitch, Rotation.Yaw - 90, Rotation.Roll},
					FUtils::GetOffsetLocation(Track, FVector{0, -TrackMeshSize.Y, 0}),
					FVector{1.0, 1.0, 1.0}
				};
				this->AddMesh(GetGenerator()->BleachersMesh, MeshTransform);

				// right bleachers
				MeshTransform = {
					FRotator{ Rotation.Pitch, Rotation.Yaw + 90, Rotation.Roll },
					FUtils::GetOffsetLocation(Track, FVector{ 0, TrackMeshSize.Y, 0 }),
					FVector{ 1.0, 1.0, 1.0 }
				};
				this->AddMesh(GetGenerator()->BleachersMesh, MeshTransform);
				BleachersWait = 6;
			}

			// watchtower
			if (Track->GetStaticMesh() == GetGenerator()->LeftCornerTrackMesh && FMath::RandRange(1, 5) == 1)
			{
				FTransform MeshTransform{
					Track->GetRelativeRotation(),
					FUtils::GetOffsetLocation(Track, FVector{CornerMeshSize.X / 4, -CornerMeshSize.Y / 2, 0}),
					FVector{1.0, 1.0, 1.0}
				};
				this->AddMesh(GetGenerator()->WatchtowerMesh, MeshTransform);
			}
		}

		// checkpoint
		if (Index - LastCheckpoint >= 30 && Track->GetStaticMesh() == GetGenerator()->ForwardTrackMesh)
		{
			FTransform MeshTransform{
				Track->GetRelativeRotation(),
				FUtils::GetOffsetLocation(Track, FVector{0.0, 0.0, 0.0}),
				FVector{1.0, 1.0, 1.0}
			};
			this->AddMesh(GetGenerator()->CheckpointMesh, MeshTransform);

			LastCheckpoint = Index;
		}

		// finish line
		if (Index == 0 || Index == TrackMeshes.Num() - 1)
		{
			FTransform MeshTransform{
				Track->GetRelativeRotation(),
				FUtils::GetOffsetLocation(Track, FVector{0.0, 0.0, 0.0}),
				FVector{1.0, 1.0, 1.0}
			};
			this->AddMesh(GetGenerator()->FinishMesh, MeshTransform);
		}
	}
}
