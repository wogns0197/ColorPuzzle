#include "CPPlayerController.h"

void ACPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;
    //InputMode.SetWidgetToFocus(MyUIWidget->TakeWidget()); // UI ������ ��Ŀ���� ����
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ���콺 ��� ����

    SetInputMode(InputMode);
    bShowMouseCursor = true;
}
