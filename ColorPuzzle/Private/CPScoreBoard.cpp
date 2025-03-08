#include "CPScoreBoard.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "CPScoreTextBlock.h"

bool UCPScoreBoard::Initialize()
{
	bool bRet = Super::Initialize();

	Default_ScoreDigit = Cast<UCPScoreTextBlock>( Default_ScoreDigit );
	if ( Default_ScoreDigit )
		TextArr.Add( Default_ScoreDigit );

	return bRet;
}

void UCPScoreBoard::SetScore( int32 Num )
{
	/// 9 -> 10 = 11
	/// 12 -> 200 = 111

	//int32 SettingDigitCount = FMath::FloorToInt(std::log10(FMath::Abs(Num))) + 1;
	//int32 PreDigitCount = FMath::FloorToInt(std::log10(FMath::Abs(PreNum))) + 1;

	TArray<int32> Digits;
	while ( Num > 0 )
	{
		Digits.Add( Num % 10 );
		Num /= 10;
	}

	if ( Digits.Num() > TextArr.Num() ) // �����Ϸ��� ���� �ڸ����� �� ũ�Ŵٸ� �����߰�
	{
		for ( const auto& el : TextArr )
		{
			el->ToggleIndexUp();
		}

		for ( int i = 0; i < Digits.Num() - TextArr.Num(); ++i )
		{
			UCPScoreTextBlock* NewScoreDigitTextBlock = CreateWidget<UCPScoreTextBlock>( GetWorld(), ScoreTextBlockObj );
			HorizontalBox_Score->AddChild( NewScoreDigitTextBlock ); // �̰� Ŭ���� �������� �Ǿ��ִ��� �� ���� �ʿ�
			TextArr.Add( NewScoreDigitTextBlock );
		}
	}

	/*for ( int i = TextArr.Num(); i > 0; i-- )
	{
		TextArr[i - 1]->SetDigit( Digits[i - 1] );
	}*/

	for ( int i = 0; i < TextArr.Num(); i++ )
	{
		TextArr[i]->SetDigit( Digits[TextArr.Num() - i - 1] );
	}

	OnScoreAccumuluated();
}

void UCPScoreBoard::SetBackgroundColor( const FLinearColor& InColor )
{
	if ( Image_Border && Image_Background )
	{
		Image_Border->SetColorAndOpacity( InColor );
		Image_Background->SetColorAndOpacity( InColor );
	}
}

void UCPScoreBoard::OnScoreAccumuluated()
{
	if ( ChangeColorAnim )
		PlayAnimation( ChangeColorAnim );
}
