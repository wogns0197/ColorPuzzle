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
	Silver,
	Gold,
	Diamond,

	COUNT,
};

USTRUCT(BlueprintType)
struct FPuzzleProbData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EPuzzleColor Color;
	UPROPERTY(EditAnywhere)
	float Weight;
};