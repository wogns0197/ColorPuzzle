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

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Debug;

public:
	virtual bool Initialize() override;
	void SetTileViewData( const TArray<TObjectPtr<class UCPPuzzleItemData>> InArr );
	void Refresh();

private:
	bool bToggleDebug = false;
private:
	UFUNCTION()
	void OnClickedDebug();
};
