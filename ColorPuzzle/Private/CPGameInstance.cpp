#include "CPGameInstance.h"
#include "CPGameMgr.h"

void UCPGameInstance::Init()
{
	Super::Init();

	if ( GameMgrClass )
	{
		GameMgr = NewObject<UCPGameMgr>( this );
	}

}

void UCPGameInstance::InitializeData( UUserWidget* pMainUI )
{
	if ( pMainUI && GameMgr )
		GameMgr->InitializeData( pMainUI, DefaultPuzzleCount, PuzzleProbData );
}