#include "CPPlayerController.h"

void ACPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;
    //InputMode.SetWidgetToFocus(MyUIWidget->TakeWidget()); // UI 위젯을 포커스로 설정
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 잠금 해제

    SetInputMode(InputMode);
    bShowMouseCursor = true;
}
