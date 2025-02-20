#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPGameInstance.generated.h"

UCLASS()
class UCPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UObject> GameMgrClass;
	UPROPERTY(EditAnywhere)
	class UDataTable* PuzzleProbData;
	UPROPERTY(EditAnywhere)
	int32 DefaultPuzzleCount;

private:
	UPROPERTY()
	class UCPGameMgr* GameMgr;

public:
	void InitializeData( UUserWidget* pMainUI );
};
