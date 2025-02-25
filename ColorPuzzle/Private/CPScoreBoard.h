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

private:
	UPROPERTY()
	TArray<class UCPScoreTextBlock*> TextArr;

	int32 PreNum = 0; // �������� ���ؼ� �����ڸ������� �����ϱ� ����

public:
	virtual bool Initialize() override;
	void SetScore( int32 Num );
};
