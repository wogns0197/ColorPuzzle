#include "CPMainUI.h"
#include "Components/TileView.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"

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