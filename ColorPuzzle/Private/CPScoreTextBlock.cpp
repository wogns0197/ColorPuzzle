#include "CPScoreTextBlock.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"


void UCPScoreTextBlock::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UCPScoreTextBlock::Initialize()
{
	bool bRet = Super::Initialize();

	bNewDigit = true;
	bIndexUp = false;
	return bRet;
}

void UCPScoreTextBlock::SetDigit( int32 Num )
{
	bool bSame = false;
	Num = FMath::Min( 9, FMath::Max( 0, Num ) );
	if ( CurDigit == Num )
		bSame = true;

	CurDigit = Num;

	if ( TextBlock_Digit )
		TextBlock_Digit->SetText( FText::AsNumber( CurDigit ) );

	if ( bNewDigit || bIndexUp || !bSame )
	{
		PlayAnimation( NewDigitAnim );
		bNewDigit = false;
	}

	if ( bIndexUp )
		bIndexUp = false;
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
