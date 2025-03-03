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

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Move0;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Move1;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Move2;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Move3;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Move4;

public:
	virtual bool Initialize() override;
	void SetTileViewData( const TArray<TObjectPtr<class UCPPuzzleItemData>> InArr );
	void Refresh();
	void SetGameMgr( TObjectPtr<class UCPGameMgr> InMgr ) { GameMgr = InMgr; }
	TObjectPtr<class UCPScoreBoard> GetScoreBoardUI() { return ScoreBoard; }
	void CallMoveAnimFinished(int nIndex);

private:
	bool bToggleDebug = false;
	TObjectPtr<class UCPGameMgr> GameMgr;

private:
	UFUNCTION()
	void OnClickedDebug();

private:
	TMap<int32, int32> MoveButtonMutexMap; // 버튼 갈기는 것 방지

	void OnClickedMovePuzzle(int32 Num);
	// 언리얼은 dynamic에 람다 안걸게 하는 정책좀 바꿔라 진짜 이게 뭐냐??
	UFUNCTION()
	void OnClickedMovePuzzle0();
	UFUNCTION()
	void OnClickedMovePuzzle1();
	UFUNCTION()
	void OnClickedMovePuzzle2();
	UFUNCTION()
	void OnClickedMovePuzzle3();
	UFUNCTION()
	void OnClickedMovePuzzle4();
};
