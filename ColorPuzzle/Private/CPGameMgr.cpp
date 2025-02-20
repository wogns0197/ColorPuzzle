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

	int32 AccumulatedWeight = 0;
	for ( const auto& el : PuzzleProbDataTable )
	{
		AccumulatedWeight += (el->Weight);
		Weights.Add( AccumulatedWeight, el->Color );
	}

	checkf( AccumulatedWeight < 100.f, TEXT("퍼즐 색상 확률 가중치 합이 100이 되지 않음"));

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

	const int32 Val = FMath::RandRange( 0, 100 );
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

void UCPGameMgr::OnEndSecondPuzzle( TWeakObjectPtr<class UCPPuzzleItemData> InSecondItemData )
{
	bDrag = false;
	if ( !pFirstItemData.IsValid() || !InSecondItemData.IsValid() ) return;

	const auto& bvValid = IsValidDrag( InSecondItemData.Get() );
	
	if ( bvValid.Key )
	{
		UE_LOG( LogTemp, Warning, TEXT( "O : Delta(%f,%f)"), bvValid.Value.X, bvValid.Value.Y );
		
		auto UpdateDataAndWidget = [=]( UCPPuzzleItemData* TargetItemData ) {
			TargetItemData->SetData( GetColorByProb() );
			TargetItemData->UpdateAnimToItemWidget();
		};

		UpdateDataAndWidget( pFirstItemData.Get() );
		UpdateDataAndWidget( InSecondItemData.Get() );


		
		pFirstItemData.Reset();
	}
	else {
		UE_LOG( LogTemp, Warning, TEXT( "X" ) );
	}
}

TPair<bool, FVector2D> UCPGameMgr::IsValidDrag( TObjectPtr<UCPPuzzleItemData> InSecondItem )
{
	TPair<bool, FVector2D> bRet = { false, FVector2D( 0,0 ) };

	if ( pFirstItemData.IsValid() && InSecondItem )
	{
		const FVector2D& FirstPos = pFirstItemData->GetPos();
		const FVector2D& SecondPos = InSecondItem->GetPos();
		if ( FirstPos == SecondPos || pFirstItemData->GetColor() != InSecondItem->GetColor()) { return bRet; }

		int32 x1 = FirstPos.X;
		int32 y1 = FirstPos.Y;
		int32 x2 = SecondPos.X;
		int32 y2 = SecondPos.Y;

		// 3개 이상으로 하려면 마우스 탈출 델타 값을 꼭 비교해야함. 그렇지 않으면 퍼즐 밖으로 마우스 조작 가능

		int32 DragDelta = -1;
		if ( (x2 - x1) != 0 && (y2 - y1) != 0 )// 대각드래그
		{
			bRet.Key = false;
		}
		else if ( (x2 - x1) == 0 && (y2 - y1) != 0 ) // 가로드래그
		{
			DragDelta = y2 - y1;
			if ( FMath::Abs( DragDelta ) < 2 ) // 바로 옆 드래그만 허용 : 코드 구조 너무 쓰레기임
			{
				bRet.Key = true;
				bRet.Value = FVector2D( x2 - x1, 0 );
			}
		}

		else if ( (x2 - x1) != 0 && (y2 - y1) == 0 ) // 세로드래그
		{
			DragDelta = x2 - x1;
			if ( FMath::Abs( DragDelta ) < 2 ) // 바로 아래 드래그만 허용 : 코드 구조 너무 쓰레기임
			{
				bRet.Key = true;
				bRet.Value = FVector2D( 0, y2 - y1 );
			}
		}

	}

	return bRet;
}