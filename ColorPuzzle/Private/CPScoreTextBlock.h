#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPScoreTextBlock.generated.h"

UCLASS()
class UCPScoreTextBlock : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_Digit;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_Effect;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* AccumulateAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* NewDigitAnim;

private:
	int32 CurDigit;
	bool bNewDigit;

public:
	virtual bool Initialize() override;
	void SetDigit( int32 Num );

public:
	FWidgetAnimationDynamicEvent AnimFinishDelegate;
	UFUNCTION()
	void OnFinishedAccumulateAnim();
};
