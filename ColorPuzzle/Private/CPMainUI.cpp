#include "CPMainUI.h"
#include "Components/TileView.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "Components/Button.h"
#include "CPGameMgr.h"
#include "CPScoreBoard.h"

bool UCPMainUI::Initialize()
{
	bool bRet = Super::Initialize();

	if ( Button_Debug && !Button_Debug->OnClicked.IsBound() )
		Button_Debug->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedDebug );

	if ( Button_Move0 && !Button_Move0->OnClicked.IsBound() )
		Button_Move0->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedMovePuzzle0 );
	if ( Button_Move1 && !Button_Move1->OnClicked.IsBound() )
		Button_Move1->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedMovePuzzle1 );
	if ( Button_Move2 && !Button_Move2->OnClicked.IsBound() )
		Button_Move2->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedMovePuzzle2 );
	if ( Button_Move3 && !Button_Move3->OnClicked.IsBound() )
		Button_Move3->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedMovePuzzle3 );
	if ( Button_Move4 && !Button_Move4->OnClicked.IsBound() )
		Button_Move4->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedMovePuzzle4 );

	for ( int i = 0; i < 5; i++ )
		MoveButtonMutexMap.Add( { i, 0 } );

	return bRet;
}

void UCPMainUI::SetTileViewData( const TArray<TObjectPtr<class UCPPuzzleItemData>> InArr )
{
	if ( TileView )
	{
		TileView->SetListItems( InArr );
	}
}

void UCPMainUI::Refresh()
{
	if ( TileView )
	{
		auto Entries = TileView->GetListItems();
		int a = 0;
		//TileView->RegenerateAllEntries();
	}
}

void UCPMainUI::OnClickedDebug()
{
	bToggleDebug = !bToggleDebug;
	
	if ( GameMgr )
		GameMgr->bDebugView = bToggleDebug;

	if ( TileView )
	{
		for ( const auto& el : TileView->GetListItems() )
		{
			if ( UCPPuzzleItemData* ItemData = Cast<UCPPuzzleItemData>( el ) )
				ItemData->OnToggleDebug( bToggleDebug );
		}
	}
}

void UCPMainUI::CallMoveAnimFinished( int nIndex )
{
	--MoveButtonMutexMap[nIndex];
}

void UCPMainUI::OnClickedMovePuzzle(int32 Num)
{
	if ( MoveButtonMutexMap[Num] > 0 ) return;

	MoveButtonMutexMap[Num] = 5;
	if ( GameMgr )
		GameMgr->MovePuzzle( Num );
}

void UCPMainUI::OnClickedMovePuzzle0()
{
	OnClickedMovePuzzle( 0 );
}
void UCPMainUI::OnClickedMovePuzzle1()
{
	OnClickedMovePuzzle( 1 );
}
void UCPMainUI::OnClickedMovePuzzle2()
{
	OnClickedMovePuzzle( 2 );
}
void UCPMainUI::OnClickedMovePuzzle3()
{

	OnClickedMovePuzzle( 3 );
}
void UCPMainUI::OnClickedMovePuzzle4()
{
	OnClickedMovePuzzle( 4 );
}