#include "CPPuzzleItem.h"
#include "CPPuzzleItemData.h"
#include "CPGameMgr.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#pragma region _NATIVE_OVERIDE
// ====================================== FOR NATIVE FUNC 

void UCPPuzzleItem::NativeOnListItemObjectSet( UObject* ListItemObject )
{
	if ( ItemData = Cast<UCPPuzzleItemData>( ListItemObject ) )
	{
		SetPuzzleStyle();
		ItemData->SetTileItem( this );
		ColorChangeTimerHandle.Invalidate();
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
	SetPuzzleStyle( true );
	PlayRefreshAnim();
}

void UCPPuzzleItem::SetPuzzleStyle( bool bDelay  )
{
	auto world = GetWorld();
	GetWorld()->GetTimerManager().SetTimer( ColorChangeTimerHandle, FTimerDelegate::CreateLambda( [=]()
	{
		// 이거 걍 함수 따로 빼서 바인딩으로 바꾸자
		if ( Image_Background )
		{
			const EPuzzleColor ColorType = ItemData->GetColor();
			FLinearColor SettingColor = PuzzleColorMap[ColorType];
			Image_Background->SetColorAndOpacity( SettingColor );

			SettingColor.A = 0.3f;
			Image_Border->SetColorAndOpacity( SettingColor );

			StopAnimation( TwinkleAnim );
			if ( IsTwinkeClass( ColorType ) ) {
				PlayAnimation( TwinkleAnim, 0.f, 0 );
			}
		}

		// for debug
		if ( TextBlock_SomeThing )
			TextBlock_SomeThing->SetText( FText::FromString( FString::Printf( TEXT("%d,%d"), 
				FMath::RoundToInt(ItemData->GetPos().X), FMath::RoundToInt(ItemData->GetPos().Y) ) ) );


	} ), 1, false, bDelay ? 0.3f : 0.f );
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