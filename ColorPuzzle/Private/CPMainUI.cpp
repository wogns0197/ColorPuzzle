#include "CPMainUI.h"
#include "Components/TileView.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "Components/Button.h"

bool UCPMainUI::Initialize()
{
	bool bRet = Super::Initialize();

	if ( Button_Debug && !Button_Debug->OnClicked.IsBound() )
		Button_Debug->OnClicked.AddDynamic( this, &UCPMainUI::OnClickedDebug );

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
	if ( TileView )
	{
		for ( const auto& el : TileView->GetListItems() )
		{
			if ( UCPPuzzleItemData* ItemData = Cast<UCPPuzzleItemData>( el ) )
				ItemData->OnToggleDebug( bToggleDebug );
		}
	}
}
