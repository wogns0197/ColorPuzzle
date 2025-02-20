#include "CPGameMgr.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "CPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "CPMainUI.h"

void UCPGameMgr::InitializeData( UUserWidget* _pMainUI, int32 nDefaultPuzzleCount, UObject* InPuzzleProbData )
{
	if ( _pMainUI )
		pMainUI = Cast<UCPMainUI>(_pMainUI);

	TArray<FPuzzleProbData*> PuzzleProbDataTable;
	UDataTable* DT_PuzzleProb = Cast<UDataTable>( InPuzzleProbData );
	if ( !DT_PuzzleProb ) return;
	DT_PuzzleProb->GetAllRows<FPuzzleProbData>( TEXT( "GetAllRows" ), PuzzleProbDataTable );

	float AccumulatedWeight = 0;
	for ( const auto& el : PuzzleProbDataTable )
	{
		AccumulatedWeight += (el->Weight);
		Weights.Add( AccumulatedWeight, el->Color );
	}

	checkf( AccumulatedWeight == 100.f, TEXT("퍼즐 색상 확률 가중치 합이 100이 되지 않음"));

	for ( int i = 0; i < nDefaultPuzzleCount; ++i )
	{
		for ( int j = 0; j < nDefaultPuzzleCount; ++j )
		{
			UCPPuzzleItemData* pData = NewObject<UCPPuzzleItemData>();
			EPuzzleColor SettingColor = GetColorByProb();
			pData->SetData( SettingColor, this, FVector2D( i, j ) );
			ItemDataArr.Emplace( pData );
		}
	}

	pMainUI->SetTileViewData( ItemDataArr );
}

EPuzzleColor UCPGameMgr::GetColorByProb()
{
	EPuzzleColor _color = EPuzzleColor::B;

	const float Val = FMath::RandRange( 0, 100 ) + FMath::RandRange( 0, 100 ) / 100.f;
	for ( const auto& el : Weights )
	{
		if ( Val <= el.Key - 1 ) {
			_color = el.Value;
			break;
		}
	}

	return _color;
}

void UCPGameMgr::OnInitFirstPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InFirstItemData )
{
	bDrag = true;
	if ( !pFirstItemData.Get() )
		pFirstItemData = InFirstItemData;
}

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

void UCPGameMgr::OnEndSecondPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InSecondItemData )
{
	bDrag = false;
	if ( !pFirstItemData.IsValid() || !InSecondItemData.IsValid() ) return;

	const auto& DragResult = IsValidDrag( InSecondItemData.Get() );
	
	if ( DragResult.TargetArr.Num() > 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "O : Delta(%f,%f)"), DragResult.Delta.X, DragResult.Delta.Y );
		
		auto UpdateDataAndWidget = [=]( UCPPuzzleItemData* TargetItemData ) {
			TargetItemData->SetData( GetColorByProb() );
			TargetItemData->UpdateAnimToItemWidget();
		};

		for ( const auto& el : DragResult.TargetArr ) {
			UpdateDataAndWidget( el.Get() );
		}
	}
	else {
		UE_LOG( LogTemp, Warning, TEXT( "X" ) );
	}

	pFirstItemData.Reset();
}

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

FDragResult UCPGameMgr::IsValidDrag( TObjectPtr<UCPPuzzleItemData> InSecondItem )
{
	FDragResult Res;

	if ( pFirstItemData.IsValid() && InSecondItem )
	{
		const FVector2D& FirstPos = pFirstItemData->GetPos();
		const FVector2D& SecondPos = InSecondItem->GetPos();
		if ( FirstPos == SecondPos || // 둘다 색이 다르고 둘다 반짝이클래스가 아닐 경우
			(pFirstItemData->GetColor() != InSecondItem->GetColor() && (!pFirstItemData->IsTwinkleItemData() && !InSecondItem->IsTwinkleItemData())) )
		{ return Res; }

		int32 x1 = FirstPos.X;
		int32 y1 = FirstPos.Y;
		int32 x2 = SecondPos.X;
		int32 y2 = SecondPos.Y;

		// 3개 이상으로 하려면 마우스 탈출 델타 값을 꼭 비교해야함. 그렇지 않으면 퍼즐 밖으로 마우스 조작 가능

		int32 DragDelta = -1;
		if ( (x2 - x1) != 0 && (y2 - y1) != 0 ) // 대각드래그
		{
			return Res;
		}
		else if ( (x2 - x1) == 0 && (y2 - y1) != 0 ) // 가로드래그
		{
			DragDelta = y2 - y1;
			if ( FMath::Abs( DragDelta ) < 5 ) // 첫 지점에서 최대 4칸 멀리까지 선택가능
			{
				// 요새는 Return Value 최적화로 복사없이 해준다네...
				Res.TargetArr = CheckBetweenValid( true, pFirstItemData->GetColor(), x1, FMath::Min( y1, y2 ), FMath::Max( y1, y2 ) );
				Res.Delta.Y = DragDelta;
			}
		}

		else if ( (x2 - x1) != 0 && (y2 - y1) == 0 ) // 세로드래그
		{
			DragDelta = x2 - x1;
			if ( FMath::Abs( DragDelta ) < 5 )
			{
				Res.TargetArr = CheckBetweenValid( false, pFirstItemData->GetColor(), y1, FMath::Min( x1, x2 ), FMath::Max( x1, x2 ) );
				Res.Delta.X = DragDelta;
			}
		}

	}

	return Res;
}

TArray<TWeakObjectPtr<class UCPPuzzleItemData>> UCPGameMgr::CheckBetweenValid( bool bHorizontal, EPuzzleColor Color, int nAnchor, int nStartPos, int nEndPos )
{
	// 첫 컬러와 마지막 컬러는 무조건 똑같을 때 이 함수로 들어옴
	// 가로, 세로 둘중 하나로 오면 배열상의 기준값이 nAnchor로옴
	// ex, (4,0) -> (4,2) 드래그 -> nAnchor = 4
	TArray<TWeakObjectPtr<UCPPuzzleItemData>> arr;
	if ( bHorizontal )
	{
		int32 CheckStartPoint = (nAnchor * 5) + nStartPos;
		for ( int i = CheckStartPoint; i < CheckStartPoint + (nEndPos - nStartPos) + 1; ++i )
		{
			EPuzzleColor TargetColor = ItemDataArr[i]->GetColor();
			if ( ItemDataArr[i]->GetColor() != Color && !IsTwinkeClass(TargetColor) ) // 반짝이 클래스는 포함시켜준다
			{
				arr.Empty(); // 버그 다 죽어라!
				break;
			}

			arr.Emplace( ItemDataArr[i] );
		}
	}

	else
	{
		int32 CheckStartPoint = (nStartPos * 5) + nAnchor;
		for ( int i = CheckStartPoint; i < CheckStartPoint + ((nEndPos - nStartPos) * 5) + 1; i += 5 )
		{
			EPuzzleColor TargetColor = ItemDataArr[i]->GetColor();
			if ( ItemDataArr[i]->GetColor() != Color && !IsTwinkeClass(TargetColor) ) // 반짝이 클래스는 포함시켜준다
			{
				arr.Empty(); // 버그 다 죽어라!
				break;
			}

			arr.Emplace( ItemDataArr[i] );
		}

	}

	return arr;
}
