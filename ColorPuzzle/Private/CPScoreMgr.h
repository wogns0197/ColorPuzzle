#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPScoreMgr.generated.h"

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

public:
	void InitializeData( TObjectPtr<class UCPScoreBoard> pInScoreBoard );
	void CalcScore( const TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CalcArr );

};
