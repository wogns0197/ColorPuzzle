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
	UPROPERTY(meta=(BindWidget))
	class UCPScoreBoard* ScoreBoard;

public:
	virtual bool Initialize() override;
	void SetTileViewData( const TArray<TObjectPtr<class UCPPuzzleItemData>> InArr );
	void Refresh();
	void SetGameMgr( TObjectPtr<class UCPGameMgr> InMgr ) { GameMgr = InMgr; }
	TObjectPtr<class UCPScoreBoard> GetScoreBoardUI() { return ScoreBoard; }

private:
	bool bToggleDebug = false;
	TObjectPtr<class UCPGameMgr> GameMgr;

private:
	UFUNCTION()
	void OnClickedDebug();
};
