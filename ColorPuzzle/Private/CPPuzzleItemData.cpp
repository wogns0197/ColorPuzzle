#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "CPGameMgr.h"

void UCPPuzzleItemData::SetData( EPuzzleColor InColor,  EPuzzleSkill InSkill, TObjectPtr<UCPGameMgr> InGameMgr, const FVector2D& InPos )
{
	Color = InColor;
	GameMgr = InGameMgr;
	Pos = InPos;
	ItemSkill = InSkill;
}

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

void UCPPuzzleItemData::OnToggleDebug( bool v )
{
	if ( EntryWidgetItem )
		EntryWidgetItem->ShowDebugInfo( v );
}