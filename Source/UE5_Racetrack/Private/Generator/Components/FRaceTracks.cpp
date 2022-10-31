#include "Generator/Components/FRaceTracks.h"
#include "Generator/AGenerator.h"

FRaceTracks::FRaceTracks(AGenerator* CurrentGenerator)
	: FGeneratorComponent{ CurrentGenerator }
{
	this->Generate();
}

void FRaceTracks::Generate()
{
	if (GetGenerator()->ForwardTrackMesh == nullptr) return;
	if (GetGenerator()->LeftCornerTrackMesh == nullptr) return;
	if (GetGenerator()->JumpTrackMesh == nullptr) return;

	const FVector ForwardMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->ForwardTrackMesh);
	const FVector CornerMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->LeftCornerTrackMesh);
	const FVector JumpMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->JumpTrackMesh);
	const FVector TerrainMeshSize = FUtils::GetStaticMeshSize(GetGenerator()->TerrainMesh);

	FRotator Rotation{0, 0, 0};
	FTransform MeshTransform {
		Rotation,
		FVector{0, (TerrainMeshSize.Y + ForwardMeshSize.Y) / 2, 0},
		FVector{1, 1, 1}
	};

	this->AddMesh(GetGenerator()->ForwardTrackMesh, MeshTransform); // add first track

	uint8 WaitForCorner = FMath::RandRange(1, GetGenerator()->TurnSpace);
	for (int Index = 0; Index < GetGenerator()->TrackLength; ++Index)
	{
		if (WaitForCorner > 0)
			--WaitForCorner;

		const bool bIsStraightSection = FMath::RandRange(1, 10) > 7;
		if (bIsStraightSection || WaitForCorner > 0) // drawn straight section || not allowed to make new corner
		{
			if (GetLastMesh()->GetStaticMesh() == GetGenerator()->LeftCornerTrackMesh) // custom tweaks for previous left/right corner
			{
				Rotation = GetLastMesh()->GetRelativeRotation();

				const bool bIsLeft = GetLastMesh()->GetRelativeScale3D().Y >= 1.0;
				const bool bNewDirection = FMath::RandBool(); // true = left
				FVector Offset{ ForwardMeshSize.X / 2 + CornerMeshSize.X / 2, -CornerMeshSize.X, 0 };
				if (!bIsLeft)
				{
					Offset.Y = -Offset.Y;
					Rotation.Yaw -= 90 + 180;
				}
				else
				{
					Rotation.Yaw -= 90;
				}

				MeshTransform = {
					Rotation,
					FUtils::GetOffsetLocation(GetLastMesh(), Offset),
					FVector{1.0, 1.0, 1.0}
				};
			}
			else // straight track
			{
				Rotation = GetLastMesh()->GetRelativeRotation();
				MeshTransform = {
					Rotation,
					FUtils::GetOffsetLocation(GetLastMesh(), FVector{ ForwardMeshSize.X, 0, 0 }),
					FVector{1.0, 1.0, 1.0}
				};
			}

			this->AddMesh(FMath::RandRange(1, 20) == 1 ? GetGenerator()->JumpTrackMesh : GetGenerator()->ForwardTrackMesh, MeshTransform);
		}
		else // corner
		{
			const FVector LastMeshSize = FUtils::GetStaticMeshSize(GetLastMesh());
			const FVector Location = FUtils::GetOffsetLocation(GetLastMesh(), FVector{ LastMeshSize.X / 2, 0, 0 });

			Rotation = GetLastMesh()->GetRelativeRotation().GetDenormalized();
			bool bNextIsLeft = FMath::RandRange(1, 10) > 5;
			
			// guarantee that we don't turn towards start line
			if (!bNextIsLeft && Rotation.Yaw >= 90)
				bNextIsLeft = true;

			if (bNextIsLeft && Rotation.Yaw >= 270)
				bNextIsLeft = false;

			MeshTransform = {
				Rotation,
				Location,
				FVector{1.0, bNextIsLeft ? 1.0 : -1.0, 1.0}
			};
			this->AddMesh(GetGenerator()->LeftCornerTrackMesh, MeshTransform);

			// instant turn
			if (FMath::RandRange(1, 5) == 1)
				WaitForCorner = 2;
			else
				WaitForCorner = FMath::RandRange(GetGenerator()->TurnSpace / 4, GetGenerator()->TurnSpace);
		}
	}
}