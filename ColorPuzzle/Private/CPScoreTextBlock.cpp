#include "CPScoreTextBlock.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

bool UCPScoreTextBlock::Initialize()
{
	bool bRet = Super::Initialize();

	if ( AccumulateAnim )
	{
		if ( !AnimFinishDelegate.IsBound() )
			AnimFinishDelegate.BindDynamic( this, &UCPScoreTextBlock::OnFinishedAccumulateAnim );

		BindToAnimationFinished( AccumulateAnim, AnimFinishDelegate );
	}

	bNewDigit = true;
	return bRet;
}

void UCPScoreTextBlock::SetDigit( int32 Num )
{
	CurDigit = FMath::Min( 9, FMath::Max( 0, Num ) );

	if ( TextBlock_Digit )
		TextBlock_Digit->SetText( FText::AsNumber( CurDigit ) );

	if ( bNewDigit )
	{
		PlayAnimation( NewDigitAnim );
		bNewDigit = false;
	}
	else
	{
		PlayAnimation( AccumulateAnim );
	}
}

//void UCPScoreTextBlock::AccumulateDigit( int32 Num /*= 1*/ )
//{
//	CurDigit += Num;
//
//	if ( TextBlock_Digit )
//		TextBlock_Digit->SetText( FText::AsNumber( CurDigit ) );
//
//	if ( AccumulateAnim )
//		PlayAnimation( AccumulateAnim );
//}

void UCPScoreTextBlock::OnFinishedAccumulateAnim()
{
	if ( TextBlock_Effect )
		TextBlock_Effect->SetText( FText::AsNumber( CurDigit ) );
}
