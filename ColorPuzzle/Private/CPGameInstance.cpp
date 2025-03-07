#include "CPGameInstance.h"
#include "CPGameMgr.h"
#include "CPMainUI.h"
#include "CPScoreMgr.h"

void UCPGameInstance::Init()
{
	Super::Init();

	if ( GameMgrClass )
	{
		GameMgr = NewObject<UCPGameMgr>( this );
	}

	if ( ScoreMgrClass )
	{
		ScoreMgr = NewObject<UCPScoreMgr>( this );
	}

}

void UCPGameInstance::InitializeData( UUserWidget* pMainUI )
{
	if ( pMainUI && ScoreMgr )
	{
		if ( UCPMainUI* pUI = Cast<UCPMainUI>( pMainUI ) ) {
			ScoreMgr->InitializeData( pUI->GetScoreBoardUI() );
		}
	}

	if ( pMainUI && GameMgr )
		GameMgr->InitializeData( pMainUI, ScoreMgr, DefaultPuzzleCount, PuzzleProbData );
}