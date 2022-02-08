// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinesweeperGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API AMinesweeperGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMinesweeperGameModeBase();
	void StartPlay() override;
	void SpawnGrid();
	void RevealTile(int x, int y);
	void MarkTileAsMine(int x , int y, bool MarkAsMine);
	void ResetGrid();
	bool GameOver;
	bool GameWon;
	void PlayBlastSound();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Audio, meta=(AllowPrivateAccess="true"))
	class USoundCue* BlastSound;

	class AGrid* GridRoot;
};
