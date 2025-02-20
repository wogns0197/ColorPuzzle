#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "CPGameMgr.h"

void UCPPuzzleItemData::InitFirstPuzzle()
{
	if ( GameMgr )
		GameMgr->OnInitFirstPuzzle( this );
}

void UCPPuzzleItemData::EndSecondPuzzle()
{
	if ( GameMgr )
		GameMgr->OnEndSecondPuzzle( this );
}

void UCPPuzzleItemData::UpdateAnimToItemWidget()
{
	if ( EntryWidgetItem )
		EntryWidgetItem->UpdatePuzzleStyle();
}


