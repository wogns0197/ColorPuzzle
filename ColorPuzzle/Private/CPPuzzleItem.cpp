#include "CPPuzzleItem.h"
#include "CPPuzzleItemData.h"
#include "CPGameMgr.h"
#include "CPMainUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

#pragma region _NATIVE_OVERIDE
// ====================================== FOR NATIVE FUNC 

void UCPPuzzleItem::NativeTick( const FGeometry& MyGeometry, float InDeltaTime )
{
	Super::NativeTick( MyGeometry, InDeltaTime );
}

void UCPPuzzleItem::NativeOnListItemObjectSet( UObject* ListItemObject )
{
	if ( ItemData = Cast<UCPPuzzleItemData>( ListItemObject ) )
	{
		PuzzleColorMap = ItemData->GetGameMgr()->GetPuzzleColorMap();
		SetStyle_Internal();
		ItemData->SetTileItem( this );
		ColorChangeTimerHandle.Invalidate();
	}

	MoveAnimEndDelegate.BindDynamic( this, &UCPPuzzleItem::OnFinishedMoveAnim );
	BindToAnimationFinished( MoveAnim, MoveAnimEndDelegate );

	bMove = false;
}

FReply UCPPuzzleItem::NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseButtonDown( InGeometry, InMouseEvent );

	if ( ItemData )
	{
		ItemData->InitFirstPuzzle();
	}

	return FReply::Handled();
}

FReply UCPPuzzleItem::NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseButtonUp( InGeometry, InMouseEvent );

	if ( ItemData )
	{
		ItemData->EndSecondPuzzle();
	}

	return FReply::Handled();
}

FReply UCPPuzzleItem::NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseMove( InGeometry, InMouseEvent );
	return FReply::Handled();
}

void UCPPuzzleItem::NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseEnter( InGeometry, InMouseEvent );
	if ( MouseEnterAnim && !bMove )
	{
		PlayAnimation( MouseEnterAnim );
	}
}

void UCPPuzzleItem::NativeOnMouseLeave( const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseLeave( InMouseEvent );
	if ( MouseLeaveAnim && !bMove )
		PlayAnimation( MouseLeaveAnim );
}

// ====================================== FOR NATIVE FUNC 
#pragma endregion

void UCPPuzzleItem::UpdatePuzzleStyle()
{
	SetPuzzleStyle();
	PlayRefreshAnim();
}

void UCPPuzzleItem::SetPuzzleStyle()
{
	auto world = GetWorld();
	GetWorld()->GetTimerManager().SetTimer( ColorChangeTimerHandle, FTimerDelegate::CreateLambda( [=]()
	{ SetStyle_Internal(); } ), 1, false, ColorChangeDelay  );
}

void UCPPuzzleItem::SetStyle_Internal()
{
	if ( !ItemData ) return;

	const EPuzzleColor ColorType = ItemData->GetColor();
	const EPuzzleSkill SkillType = ItemData->GetItemSkill();

	if ( Image_Background )
	{
		FLinearColor SettingColor = PuzzleColorMap[ColorType];
		Image_Background->SetColorAndOpacity( SettingColor );

		SettingColor.A = 0.3f;
		Image_Border->SetColorAndOpacity( SettingColor );

		StopAnimation( TwinkleAnim );
		if ( IsTwinkeColor( ColorType ) ) {
			PlayAnimation( TwinkleAnim, 0.f, 0 );
		}
	}

	if ( Image_SkillIcon )
	{
		if ( SkillType != EPuzzleSkill::Default )
		{
			const FSlateBrush& SkillBrush = SkillBrushMap[SkillType];
			Image_SkillIcon->SetBrush( SkillBrush );
		}
		Image_SkillIcon->SetVisibility( SkillType == EPuzzleSkill::Default ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible );
	}
}

void UCPPuzzleItem::OnFinishedMoveAnim()
{
	RenderTransform.Translation = FVector2D( 0, 0 );
	SetRenderTransform( RenderTransform );
	InvalidateLayoutAndVolatility();
	SetStyle_Internal();
	bMove = false;

	if ( ItemData )
	{
		auto pMainUI = ItemData->GetGameMgr()->GetMainUI();
		pMainUI->CallMoveAnimFinished( ItemData->GetPos().Y );
	}
}

void UCPPuzzleItem::ShowDebugInfo( bool v )
{
	// for debug
	if ( TextBlock_SomeThing )
	{
		TextBlock_SomeThing->SetText( FText::FromString( FString::Printf( TEXT( "%d,%d" ),
			FMath::RoundToInt( ItemData->GetPos().X ), FMath::RoundToInt( ItemData->GetPos().Y ) ) ) );

		TextBlock_SomeThing->SetVisibility( v ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
	}
}

void UCPPuzzleItem::PlayMoveAnim()
{
	if ( MoveAnim )
	{
		PlayAnimation( MoveAnim );
		bMove = true;
	}
}

void UCPPuzzleItem::PlayRefreshAnim()
{
	PlayAnimation( RefreshAnim );
}

void UCPPuzzleItem::PlayDragFailAnim()
{
	PlayAnimation( DragFailAnim );
}
