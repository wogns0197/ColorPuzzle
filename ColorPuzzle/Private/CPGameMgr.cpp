#include "CPGameMgr.h"
#include "CPPuzzleItemData.h"
#include "CPPuzzleItem.h"
#include "CPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "CPMainUI.h"
#include "CPScoreBoard.h"
#include "CPScoreMgr.h"

void UCPGameMgr::InitializeData( UUserWidget* _pMainUI, TObjectPtr<UCPScoreMgr> _pScoreMgr,int32 nDefaultPuzzleCount, UObject* InPuzzleProbData )
{
	if ( _pMainUI ) {
		pMainUI = Cast<UCPMainUI>( _pMainUI );
		pMainUI->SetGameMgr( this );
	}

	if ( _pScoreMgr )
		ScoreMgr = _pScoreMgr;

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
			const EPuzzleColor SettingColor = GetColorByProb();
			const EPuzzleSkill SettingSkill = GetSkillByProb( SettingColor );
			pData->SetData( SettingColor, SettingSkill, this, FVector2D( i, j ) );
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

	// for debug
	if ( bDebugView ) {
		TArray<EPuzzleColor> DebugColorArr = { EPuzzleColor::Gold, EPuzzleColor::Diamond };
		_color = DebugColorArr[FMath::RandRange( 0, 1 )];
	}

	return _color;
}

EPuzzleSkill UCPGameMgr::GetSkillByProb( EPuzzleColor InColor )
{
	EPuzzleSkill _skill = GetSkillByColor( InColor );

	if ( _skill != EPuzzleSkill::HVRandom ) return _skill;

	TArray<EPuzzleSkill> tempArr{ EPuzzleSkill::Horizon, EPuzzleSkill::Vertical };
	return tempArr[FMath::RandRange( 0, tempArr.Num() - 1 )];
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
	
	if ( DragResult.bValid && DragResult.TargetArr.Num() > 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "O : Delta(%f,%f)"), DragResult.Delta.X, DragResult.Delta.Y );
		
		auto UpdateDataAndWidget = [=]( UCPPuzzleItemData* TargetItemData ) {
			const EPuzzleColor SettingColor = GetColorByProb();
			const EPuzzleSkill SettingSkill = GetSkillByProb( SettingColor );
			TargetItemData->SetData( SettingColor, SettingSkill );
			TargetItemData->UpdateAnimToItemWidget();
		};

		for ( const auto& el : DragResult.TargetArr ) {
			UpdateDataAndWidget( el.Get() );
		}

		if ( ScoreMgr )
		{
			ScoreMgr->CalcScore( DragResult.TargetArr );
			/*Score += DragResult.TargetArr.Num();
			pScoreBoard->SetScore( Score );*/
		}
	}

	if ( !DragResult.bValid )
	{
		if ( DragResult.Delta.X == -1 && DragResult.Delta.Y == -1 ) // 대각 드래그는 first와 second만 흔들도록함
		{
			pFirstItemData->OnDragFailed();
			InSecondItemData->OnDragFailed();
		}
		else
		{
			const auto& DeltaPuzzles = GetDeltaPuzzles( DragResult.Delta.Y > 0 ? true : false, pFirstItemData->GetPos(), InSecondItemData->GetPos() );
			for ( const auto& el : DeltaPuzzles )
			{
				el->OnDragFailed();
			}
		}
	}

	pFirstItemData.Reset();
}

void UCPGameMgr::MovePuzzle( int32 nIndex, bool bDown )
{
	FPuzzleData PreData = ItemDataArr[nIndex + 20]->GetData();
	for ( int i = nIndex; i < 25; i += 5 )
	{
		if ( UCPPuzzleItemData* pItemdata = ItemDataArr[i] )
		{
			pItemdata->SetDataDirty( PreData );
			pItemdata->OnProcessMove();
			PreData = pItemdata->GetData();
			pItemdata->FlushData();
		}
	}
}

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

FDragResult UCPGameMgr::IsValidDrag( TObjectPtr<UCPPuzzleItemData> InSecondItem )
{
	FDragResult Res;
	Res.bValid = false;
	if ( pFirstItemData.IsValid() && InSecondItem )
	{
		const FVector2D& FirstPos = pFirstItemData->GetPos();
		const FVector2D& SecondPos = InSecondItem->GetPos();

		if ( ( FirstPos == SecondPos ) )
		{
			if ( IsTwinkeClass( pFirstItemData->GetColor() ) )
			{
				return UseSkill( pFirstItemData.Get() );
			}
		}

		int32 x1 = FirstPos.X;
		int32 y1 = FirstPos.Y;
		int32 x2 = SecondPos.X;
		int32 y2 = SecondPos.Y;

		int32 DragDelta = -1;
		if ( (x2 - x1) != 0 && (y2 - y1) != 0 ) // 대각드래그
		{
			Res.Delta = FVector2D( -1, -1 );
			return Res;
		}

		if ( auto pTwinkle = IsAnyOfTwinkleClass( pFirstItemData.Get(), InSecondItem ) )
		{
			return UseSkill( pTwinkle );
		}

		// 둘다 색이 다르고 둘다 반짝이클래스가 아닐 경우
		/*else if ( pFirstItemData->GetColor() != InSecondItem->GetColor() && (!pFirstItemData->IsTwinkleItemData() && !InSecondItem->IsTwinkleItemData()) )
		{ 
			return Res; 
		}*/
		
		if ( (x2 - x1) == 0 && (y2 - y1) != 0 ) // 가로드래그
		{
			DragDelta = FMath::Abs( y2 - y1 );
			if ( DragDelta < 5 ) // 첫 지점에서 최대 4칸 멀리까지 선택가능
			{
				// 요새는 Return Value 최적화로 복사없이 해준다네...
				Res.TargetArr = CheckBetweenValid( true, pFirstItemData->GetColor(), x1, FMath::Min( y1, y2 ), FMath::Max( y1, y2 ) );
				if ( Res.TargetArr.Num() > 0 ) Res.bValid = true;
				Res.Delta.Y = DragDelta;
			}
		}

		else if ( (x2 - x1) != 0 && (y2 - y1) == 0 ) // 세로드래그
		{
			DragDelta = FMath::Abs( x2 - x1 );
			if ( DragDelta < 5 )
			{
				Res.TargetArr = CheckBetweenValid( false, pFirstItemData->GetColor(), y1, FMath::Min( x1, x2 ), FMath::Max( x1, x2 ) );
				if ( Res.TargetArr.Num() > 0 ) Res.bValid = true;
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

TArray<TWeakObjectPtr<class UCPPuzzleItemData>> UCPGameMgr::GetDeltaPuzzles( bool bHorizontal, FVector2D startPos, FVector2D endPos )
{
	TArray<TWeakObjectPtr<class UCPPuzzleItemData>> Res;

	int32 startPivot = FMath::Min( startPos.Y + ( startPos.X * 5 ), endPos.Y + ( endPos.X * 5 ) );
	int32 endPivot = FMath::Max( startPos.Y + ( startPos.X * 5 ), endPos.Y + ( endPos.X * 5 ) );
	for ( int i = startPivot; i < endPivot + 1; bHorizontal ? i++ : i += 5 )
	{
			Res.AddUnique( ItemDataArr[i] );
	}

	return Res;
}

TObjectPtr<UCPPuzzleItemData> UCPGameMgr::IsAnyOfTwinkleClass( TObjectPtr< UCPPuzzleItemData> pFirst, TObjectPtr< UCPPuzzleItemData> pSecond )
{
	if ( IsTwinkeClass( pFirst->GetColor() ) ) return pFirst;
	if ( IsTwinkeClass( pSecond->GetColor() ) ) return pSecond;
	return nullptr;
}

FDragResult UCPGameMgr::UseSkill( TObjectPtr< UCPPuzzleItemData> SkillItemdata, EPuzzleSkill InSkill /*= EPuzzleSkill::Default*/ )
{
	FDragResult Res;
	Res.bValid = true;
	const FVector2D& TargetPos = SkillItemdata->GetPos();

	EPuzzleSkill SkillType = SkillItemdata->GetItemSkill();
	if ( InSkill != EPuzzleSkill::Default ) SkillType = InSkill;

	switch ( SkillType )
	{
	case EPuzzleSkill::Horizon:
	{
		for ( int i = TargetPos.X * 5; i < (TargetPos.X * 5) + 5; ++i )
		{
			Res.TargetArr.AddUnique( ItemDataArr[i] );
		}

		break;
	}
	case EPuzzleSkill::Vertical:
	{
		for ( int i = TargetPos.Y; i < TargetPos.Y + 20 + 1; i += 5 )
		{
			Res.TargetArr.AddUnique( ItemDataArr[i] );
		}
		break;
	}
	case EPuzzleSkill::AllSide:
	{
		Res = UseSkill( SkillItemdata, EPuzzleSkill::Vertical );
		Res.TargetArr.Remove( SkillItemdata );
		Res.TargetArr.Append( UseSkill( SkillItemdata, EPuzzleSkill::Horizon ).TargetArr );
		break;
	}
	case EPuzzleSkill::RoundRange:
	{
		for ( int i = FMath::Max( 0, TargetPos.X - 1 ); i < FMath::Min( 4, TargetPos.X + 1 ) + 1; ++i )
		{
			for ( int j = FMath::Max( 0, TargetPos.Y - 1 ); j < FMath::Min( 4, TargetPos.Y + 1 ) + 1; ++j )
			{
				Res.TargetArr.AddUnique( ItemDataArr[(i * 5) + j] );
			}
		}
		break;
	}
	default:
		break;
	}

	return Res;
}
