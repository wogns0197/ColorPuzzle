#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonEnum.h"
#include "CPGameMgr.generated.h"

USTRUCT()
struct FDragResult
{
	GENERATED_BODY()
	// 아으~~~ 이럴거면 그냥 첨부터 pair, set 쓰지말자~~~
	bool bValid;
	FVector2D Delta;
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> TargetArr;

	FDragResult() : Delta( 0 )
	{
		TargetArr.Empty();
	}
};

UCLASS(Blueprintable)
class UCPGameMgr : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<class UCPMainUI> pMainUI;
	UPROPERTY()
	TArray<TObjectPtr<class UCPPuzzleItemData>> ItemDataArr;
	UPROPERTY(EditAnywhere)
	TMap<EPuzzleColor, FLinearColor> PuzzleColorMap;

private:
	TMap<float, EPuzzleColor> Weights;
	UPROPERTY()
	TObjectPtr<class UCPScoreMgr> ScoreMgr;

public:
	bool bDebugView = false; // for debug
	void InitializeData( UUserWidget* _pMainUI, TObjectPtr<class UCPScoreMgr> _pScoreMgr,int32 nDefaultPuzzleCount, UObject* InPuzzleProbData );
	UCPMainUI* GetMainUI() { return pMainUI; }

// UI 컨트롤
private:
	bool bDrag = false;
	TWeakObjectPtr<class UCPPuzzleItemData> pFirstItemData;
	TWeakObjectPtr<class UCPPuzzleItemData> pSecondItemData;
	TArray<TObjectPtr<class UCPPuzzleItemData>> TwoClickPuzzleArr;

private:
	FDragResult IsValidDrag();
	FDragResult UseSkill( TObjectPtr<class UCPPuzzleItemData> SkillItemdata, EPuzzleSkill InSkill = EPuzzleSkill::Default );
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CheckBetweenValid( bool bHorizontal, EPuzzleColor Color, int nAnchor, int nStartPos, int nEndPos );
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> GetDeltaPuzzles( bool bHorizontal, FVector2D startPos, FVector2D endPos );
	TObjectPtr<class UCPPuzzleItemData> IsAnyOfTwinkleClass( TObjectPtr<class UCPPuzzleItemData> pFirst, TObjectPtr<class UCPPuzzleItemData> pSecond );
	void DoMerge( const FDragResult& Res );

	void RegisterClickedPuzzleProc( TObjectPtr<class UCPPuzzleItemData> InPuzzle );
	void IsValidClick();
	void ResetWeakPuzzlePtr();

public:
	void OnInitFirstPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InFirstItemData );
	void OnEndSecondPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InSecondItemData );
	void MovePuzzle( int32 nIndex, bool bDown = true );
	bool IsDragging() { return bDrag; }
	const TMap<EPuzzleColor, FLinearColor>& GetPuzzleColorMap() { return PuzzleColorMap; }

private:
	EPuzzleColor GetColorByProb();
	EPuzzleSkill GetSkillByProb( EPuzzleColor InColor );
};
