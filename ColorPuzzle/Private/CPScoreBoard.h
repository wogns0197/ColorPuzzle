#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPScoreBoard.generated.h"

UCLASS()
class UCPScoreBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCPScoreTextBlock> ScoreTextBlockObj;
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* HorizontalBox_Score;
	UPROPERTY(meta=(BindWidget))
	class UCPScoreTextBlock* Default_ScoreDigit;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Border;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Background;

	UPROPERTY( meta = (BindWidgetAnim), Transient )
	class UWidgetAnimation* ChangeColorAnim;

private:
	UPROPERTY()
	TArray<class UCPScoreTextBlock*> TextArr;

	int32 PreNum = 0; // 이전값과 비교해서 오른자리수에만 대응하기 위해

public:
	virtual bool Initialize() override;
	void SetScore( int32 Num ); // 이 함수에 색 지정까지 일부러 포함안시킴 : 나중에 분리가 필요해질 때를 고려함
	void SetBackgroundColor( const FLinearColor& InColor );
};
