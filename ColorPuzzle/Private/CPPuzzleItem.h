#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.generated.h"

UCLASS()
class UCPPuzzleItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Background;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Border;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_SkillIcon;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_SomeThing;

	UPROPERTY(EditAnywhere)
	TMap<EPuzzleSkill, FSlateBrush> SkillBrushMap;
	UPROPERTY(EditAnywhere)
	float ColorChangeDelay;


	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* RefreshAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* TwinkleAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* MoveAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* MouseEnterAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* MouseLeaveAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* DragFailAnim;

	UPROPERTY(EditAnywhere)
	TMap<EPuzzleColor, FLinearColor> PuzzleColorMap;

private:
	FTimerHandle ColorChangeTimerHandle;
	bool bMove;

public:
	virtual void NativeTick( const FGeometry& MyGeometry, float InDeltaTime ) override;
	virtual void NativeOnListItemObjectSet( UObject* ListItemObject ) override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

public:
	void UpdatePuzzleStyle();
	void SetPuzzleStyle();
	void ShowDebugInfo(bool v);
	void PlayMoveAnim();
	void PlayDragFailAnim();

private:
	void PlayRefreshAnim();
	void SetStyle_Internal();

public:
	UPROPERTY()
	TObjectPtr<class UCPPuzzleItemData> ItemData; // safe..??

private:
	FWidgetAnimationDynamicEvent MoveAnimEndDelegate;
	UFUNCTION()
	void OnFinishedMoveAnim();
};
