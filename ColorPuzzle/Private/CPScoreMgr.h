#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPScoreMgr.generated.h"

enum class EPuzzleColor;

UCLASS()
class UCPScoreMgr : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UCPScoreBoard> pScoreBoard;

private:
	UPROPERTY()
	int32 Score;

	EPuzzleColor PreScoredColor;
	TMap<EPuzzleColor, FLinearColor> PuzzleColorMap;

public:
	void InitializeData( TObjectPtr<class UCPScoreBoard> pInScoreBoard, const TMap<EPuzzleColor, FLinearColor>& InColorMap );
	void CalcScore( const TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CalcArr );

private:
	void SetScoreBoardColor( const FLinearColor& InColor );
	void CheckValidCombo();
};
