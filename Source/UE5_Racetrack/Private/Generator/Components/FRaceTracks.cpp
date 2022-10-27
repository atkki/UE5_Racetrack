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

	FRotator Rotation{0, 0, 0};
	FTransform MeshTransform {
		Rotation,
		FVector{0, ForwardMeshSize.Y, 0},
		FVector{1, 1, 1}
	};

	this->AddMesh(GetGenerator()->ForwardTrackMesh, MeshTransform);

	uint8 WaitForCorner = FMath::RandRange(1, GetGenerator()->TurnSpace);
	for (int k = 0; k < GetGenerator()->TrackLength; ++k)
	{
		if (WaitForCorner > 0)
			--WaitForCorner;

		const bool bIsStraightSection = FMath::RandRange(1, 10) > 7;
		if (bIsStraightSection || WaitForCorner > 0)
		{
			if (GetLastMesh()->GetStaticMesh() == GetGenerator()->LeftCornerTrackMesh)
			{
				Rotation = GetLastMesh()->GetRelativeRotation();

				bool bIsLeft = GetLastMesh()->GetRelativeScale3D().Y >= 1.0;
				bool bNewDirection = FMath::RandBool(); // true = left
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
					Rotation, // Rotation
					FUtils::GetOffsetLocation(GetLastMesh(), Offset), // Position
					FVector{1.0, 1.0, 1.0} // Scale
				};
			}
			else
			{
				Rotation = GetLastMesh()->GetRelativeRotation();
				MeshTransform = {
					Rotation, // Rotation
					FUtils::GetOffsetLocation(GetLastMesh(), FVector{ ForwardMeshSize.X, 0, 0 }), // Position
					FVector{1.0, 1.0, 1.0} // Scale
				};
			}

			this->AddMesh(FMath::RandRange(1, 20) == 1 ? GetGenerator()->JumpTrackMesh : GetGenerator()->ForwardTrackMesh, MeshTransform);
		}
		else
		{
			const FVector LastMeshSize = FUtils::GetStaticMeshSize(GetLastMesh());
			FVector Location = FUtils::GetOffsetLocation(GetLastMesh(), FVector{ LastMeshSize.X / 2, 0, 0 });

			Rotation = GetLastMesh()->GetRelativeRotation().GetDenormalized();
			bool bNextIsLeft = FMath::RandRange(1, 10) > 5;
			
			// guarantee that we don't turn towards start line
			if (!bNextIsLeft && Rotation.Yaw >= 90)
				bNextIsLeft = true;

			if (bNextIsLeft && Rotation.Yaw >= 270)
				bNextIsLeft = false;

			MeshTransform = {
				Rotation, // Rotation
				Location,
				FVector{1.0, bNextIsLeft ? 1.0 : -1.0, 1.0} // Scale
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