#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonEnum.generated.h"

UENUM()
enum class EPuzzleColor : int32
{
	R,
	G,
	B,
	Yellow,
	White,
	Silver,
	Gold,
	Diamond,

	COUNT,
};

static bool IsTwinkeClass( EPuzzleColor c )
{
	switch ( c )
	{
	case EPuzzleColor::Silver:
	case EPuzzleColor::Gold:
	case EPuzzleColor::Diamond:
		return true;
	default:
		break;
	}
	return false;
}

USTRUCT(BlueprintType)
struct FPuzzleProbData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EPuzzleColor Color;
	UPROPERTY(EditAnywhere)
	float Weight;
};