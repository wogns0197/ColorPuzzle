#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonEnum.generated.h"

USTRUCT()
struct FPuzzleData
{
	GENERATED_BODY()
	EPuzzleColor Color;
	EPuzzleSkill ItemSkill;
	FVector2D Pos;
};

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

UENUM()
enum class EPuzzleSkill : int32
{
	Default,

	/*Left,
	Right,*/ // ���ҽ��� ������ ���߿� Ȯ������

	Horizon,
	Vertical,
	HVRandom,

	AllSide,
	RoundRange,

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

static EPuzzleSkill GetSkillByColor( EPuzzleColor c )
{
	switch ( c )
	{
	case EPuzzleColor::R:
	case EPuzzleColor::G:
	case EPuzzleColor::B:
	case EPuzzleColor::Yellow:
	case EPuzzleColor::White:
	{
		return EPuzzleSkill::Default;
	}
	case EPuzzleColor::Silver:
		return EPuzzleSkill::HVRandom;
	case EPuzzleColor::Gold:
		return EPuzzleSkill::RoundRange;
	case EPuzzleColor::Diamond:
		return EPuzzleSkill::AllSide;
	default:
		return EPuzzleSkill::Default;
	}
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