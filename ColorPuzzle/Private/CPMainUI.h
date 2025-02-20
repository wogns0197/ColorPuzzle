#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPMainUI.generated.h"

UCLASS()
class UCPMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTileView* TileView;

public:
	void SetTileViewData( const TArray<TObjectPtr<class UCPPuzzleItemData>> InArr );
	void Refresh();
};
