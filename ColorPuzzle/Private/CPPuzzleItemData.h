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
	EPuzzleSkill ItemSkill;
	FVector2D Pos;

	UPROPERTY()
	TObjectPtr<class UCPGameMgr> GameMgr;

	UPROPERTY()
	TObjectPtr<class UCPPuzzleItem> EntryWidgetItem;
	
public:
	void SetData( EPuzzleColor InColor, EPuzzleSkill InSkill, TObjectPtr<class UCPGameMgr> InGameMgr, const FVector2D& InPos );
	void SetData( EPuzzleColor InColor, EPuzzleSkill InSkill )
	{
		Color = InColor;
		ItemSkill = InSkill;
	}

	void SetTileItem( TObjectPtr<class UCPPuzzleItem> InTileItem ) { EntryWidgetItem = InTileItem; }

	bool IsTwinkleItemData() { return IsTwinkeClass( Color ); }
	EPuzzleColor GetColor() { return Color; }
	const FVector2D& GetPos() { return Pos; }
	TObjectPtr<class UCPGameMgr> GetGameMgr() { return GameMgr; }
	EPuzzleSkill GetItemSkill() { return ItemSkill; }

	void InitFirstPuzzle();
	void EndSecondPuzzle();
	void UpdateAnimToItemWidget();

	void OnToggleDebug( bool v ); // for debug
};
