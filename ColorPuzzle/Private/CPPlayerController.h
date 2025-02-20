#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPPlayerController.generated.h"

UCLASS()
class ACPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
