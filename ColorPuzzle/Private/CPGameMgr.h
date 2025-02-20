#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonEnum.h"
#include "CPGameMgr.generated.h"

UCLASS()
class UCPGameMgr : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<class UCPMainUI> pMainUI;
	UPROPERTY()
	TArray<TObjectPtr<class UCPPuzzleItemData>> ItemDataArr;

private:
	TMap<float, EPuzzleColor> Weights;

public:
	void InitializeData( UUserWidget* _pMainUI, int32 nDefaultPuzzleCount, UObject* InPuzzleProbData );

// UI 컨트롤
private:
	bool bDrag = false;
	TWeakObjectPtr<class UCPPuzzleItemData> pFirstItemData;

private:
	TPair<bool, FVector2D> IsValidDrag( TObjectPtr<class UCPPuzzleItemData> InSecondItemData );

public:
	void OnInitFirstPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InFirstItemData );
	void OnEndSecondPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InSecondItemData );


private:
	EPuzzleColor GetColorByProb();
};
