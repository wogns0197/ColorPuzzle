#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPHudBase.generated.h"


UCLASS()
class ACPHudBase : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainUIClass;

public:
	virtual void BeginPlay() override;

};
