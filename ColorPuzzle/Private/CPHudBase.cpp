#include "CPHudBase.h"
#include "CPGameInstance.h"
#include "Blueprint/UserWidget.h"

void ACPHudBase::BeginPlay()
{
	if ( !MainUIClass ) return;

	UUserWidget* MainUI = CreateWidget<UUserWidget>( GetWorld(), MainUIClass );
	if ( MainUI )
	{
		MainUI->AddToViewport();
	}

	if ( auto GI = GetGameInstance<UCPGameInstance>() )
	{
		GI->InitializeData( MainUI );
	}
}
