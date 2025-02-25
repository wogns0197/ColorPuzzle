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
	FVector2D Delta;
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> TargetArr;

	FDragResult() : Delta( 0 )
	{
		TargetArr.Empty();
	}
};

UCLASS()
class UCPGameMgr : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<class UCPMainUI> pMainUI;
	UPROPERTY()
	TObjectPtr<class UCPScoreBoard> pScoreBoard;
	UPROPERTY()
	TArray<TObjectPtr<class UCPPuzzleItemData>> ItemDataArr;

private:
	TMap<float, EPuzzleColor> Weights;
	int32 Score;

public:
	bool bDebugView = false; // for debug
	void InitializeData( UUserWidget* _pMainUI, int32 nDefaultPuzzleCount, UObject* InPuzzleProbData );

// UI 컨트롤
private:
	bool bDrag = false;
	TWeakObjectPtr<class UCPPuzzleItemData> pFirstItemData;

private:
	FDragResult IsValidDrag( TObjectPtr<class UCPPuzzleItemData> InSecondItemData );
	FDragResult UseSkill( TObjectPtr<class UCPPuzzleItemData> SkillItemdata, EPuzzleSkill InSkill = EPuzzleSkill::Default );
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CheckBetweenValid( bool bHorizontal, EPuzzleColor Color, int nAnchor, int nStartPos, int nEndPos );

public:
	void OnInitFirstPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InFirstItemData );
	void OnEndSecondPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InSecondItemData );


private:
	EPuzzleColor GetColorByProb();
	EPuzzleSkill GetSkillByProb( EPuzzleColor InColor );
};
