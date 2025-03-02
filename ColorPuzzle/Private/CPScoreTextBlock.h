#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPScoreTextBlock.generated.h"

UCLASS()
class UCPScoreTextBlock : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY( meta = (BindWidget) )
	class UTextBlock* TextBlock_Digit;
	UPROPERTY( meta = (BindWidget) )
	class UTextBlock* TextBlock_Effect;

	UPROPERTY( meta = (BindWidgetAnim), Transient )
	class UWidgetAnimation* NewDigitAnim;

private:
	int32 CurDigit;
	bool bNewDigit;
	bool bIndexUp; // �ڸ��� ���� üũ

public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	void SetDigit( int32 Num );
	void ToggleIndexUp() { bIndexUp = true; }

};
