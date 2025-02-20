#include "CPPuzzleItem.h"
#include "CPPuzzleItemData.h"
#include "CPGameMgr.h"
#include "Components/Image.h"

#pragma region _NATIVE_OVERIDE
// ====================================== FOR NATIVE FUNC 
void UCPPuzzleItem::NativeOnListItemObjectSet( UObject* ListItemObject )
{
	if ( ItemData = Cast<UCPPuzzleItemData>( ListItemObject ) )
	{
		SetPuzzleStyle();
		ItemData->SetTileItem( this );
	}
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
	if ( Image_Background )
	{
		FLinearColor SettingColor = PuzzleColorMap[ItemData->GetColor()];
		Image_Background->SetColorAndOpacity( SettingColor );

		SettingColor.A = 0.3f;
		Image_Border->SetColorAndOpacity( SettingColor );
	}
}

void UCPPuzzleItem::PlayRefreshAnim()
{
	PlayAnimation( RefreshAnim );
}

void UCPPuzzleItem::NativeOnMouseLeave( const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseLeave( InMouseEvent );
	UE_LOG( LogTemp, Log, TEXT("Leave %d, Delta(%f,%f)"), 
		ItemData->GetColor(), InMouseEvent.GetCursorDelta().X, InMouseEvent.GetCursorDelta().Y);
}