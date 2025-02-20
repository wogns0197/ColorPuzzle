#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonEnum.h"
#include "CPPuzzleItemData.generated.h"

UCLASS()
class UCPPuzzleItemData : public UObject
{
	GENERATED_BODY()
	
private:
	EPuzzleColor Color;
	FVector2D Pos;

	UPROPERTY()
	TObjectPtr<class UCPGameMgr> GameMgr;

	UPROPERTY()
	TObjectPtr<class UCPPuzzleItem> EntryWidgetItem;

public:
	void SetData( EPuzzleColor InColor, TObjectPtr<class UCPGameMgr> InGameMgr
		, const FVector2D& InPos )
	{
		Color = InColor;
		GameMgr = InGameMgr;
		Pos = InPos;
	}
	void SetData( EPuzzleColor InColor )
	{
		Color = InColor;
	}

	void SetTileItem( TObjectPtr<class UCPPuzzleItem> InTileItem ) { EntryWidgetItem = InTileItem; }

	EPuzzleColor GetColor() { return Color; }
	const FVector2D& GetPos() { return Pos; }
	TObjectPtr<class UCPGameMgr> GetGameMgr() { return GameMgr; }

	void InitFirstPuzzle();
	void EndSecondPuzzle();
	void UpdateAnimToItemWidget();
};
