#include "CPScoreMgr.h"
#include "CPScoreBoard.h"

void UCPScoreMgr::InitializeData( TObjectPtr<UCPScoreBoard> pInScoreBoard )
{
	pScoreBoard = pInScoreBoard;
	Score = 0;
}

void UCPScoreMgr::CalcScore( const TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CalcArr )
{
	// todo
	Score += FMath::Max(0, CalcArr.Num());

	pScoreBoard->SetScore( Score );
}
