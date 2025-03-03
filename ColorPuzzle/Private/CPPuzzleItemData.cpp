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

void UCPPuzzleItemData::SetDataDirty(  FPuzzleData InData, bool bExcepPos /*= true*/ )
{
	if ( bExcepPos )
	{
		DirtyData.Color = InData.Color;
		DirtyData.ItemSkill = InData.ItemSkill;
		DirtyData.Pos = this->Pos; // 명시적으로라도 적어둔단
	}
	else
		DirtyData = InData;
}

void UCPPuzzleItemData::FlushData()
{
	Color = DirtyData.Color;
	ItemSkill = DirtyData.ItemSkill;
	Pos = DirtyData.Pos;
}

FPuzzleData UCPPuzzleItemData::GetData()
{
	FPuzzleData fRet;
	fRet.Color = Color;
	fRet.ItemSkill = ItemSkill;
	fRet.Pos = Pos;

	return fRet;
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

void UCPPuzzleItemData::OnProcessMove()
{
	if ( EntryWidgetItem )
		EntryWidgetItem->PlayMoveAnim();
}
