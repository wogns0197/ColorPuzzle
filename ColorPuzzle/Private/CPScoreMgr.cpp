#include "CPScoreMgr.h"
#include "CPScoreBoard.h"
#include "CPPuzzleItemData.h"
#include "CommonEnum.h"

void UCPScoreMgr::InitializeData( TObjectPtr<UCPScoreBoard> pInScoreBoard, const TMap<EPuzzleColor, FLinearColor>& InColorMap )
{
	pScoreBoard = pInScoreBoard;
	Score = 0;
	PreScoredColor = EPuzzleColor::NONE;
	PuzzleColorMap = InColorMap;
}

void UCPScoreMgr::CalcScore( const TArray<TWeakObjectPtr<class UCPPuzzleItemData>> CalcArr )
{
	for ( const auto& el : CalcArr )
	{
		if ( IsTwinkeColor( el->GetColor() ) ) {
			break;
		}
		else {
			SetScoreBoardColor( PuzzleColorMap[el->GetColor()] );
			break;
		}
	}


	// todo
	Score += FMath::Max(0, CalcArr.Num());

	pScoreBoard->SetScore( Score );
}

void UCPScoreMgr::SetScoreBoardColor( const FLinearColor& InColor )
{
	pScoreBoard->SetBackgroundColor( InColor );
}

void UCPScoreMgr::CheckValidCombo()
{
}
