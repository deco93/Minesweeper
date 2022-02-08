// Copyright Epic Games, Inc. All Rights Reserved.


#include "MinesweeperGameModeBase.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Grid.h"


AMinesweeperGameModeBase::AMinesweeperGameModeBase()
{
	DefaultPawnClass = nullptr;
	GameWon = false;
	GameOver = false;
	static ConstructorHelpers::FObjectFinder<USoundCue> BlastSoundCue(TEXT("/Game/Sounds/blast_Cue"));
	if (BlastSoundCue.Succeeded())
	{
		BlastSound = BlastSoundCue.Object;
	}
	
}

void AMinesweeperGameModeBase::StartPlay()
{
	Super::StartPlay();
	FVector SpawnLocation(0, 0, 2000);
	FRotator SpawnRotation(-90, 0, 0);
	ACameraActor* PlayerViewCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, FActorSpawnParameters());
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerViewCamera->SetOwner(PlayerController);
		PlayerController->SetViewTarget(PlayerViewCamera);
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
	SpawnGrid();
}

void AMinesweeperGameModeBase::SpawnGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnGrid Called"));
	//set location correctly for multiple spawned Tiles here instead in Grid constructor
	FTransform GridTransform = FTransform(FRotator(),FVector(), FVector(1,1,1));
	GridRoot = GetWorld()->SpawnActor<AGrid>(AGrid::StaticClass(), GridTransform, FActorSpawnParameters());
	//GridRoot->SpawnTiles();
}

void AMinesweeperGameModeBase::RevealTile(int x, int y)
{
	ATile* Tile = GridRoot->GetTile(x, y);
	/*if (Tile->TileType == MINE)
	{
		Tile->Mesh->SetMaterial(0, GridRoot->MineUMaterial);
		//TODO Update UI to say GAME OVER
		UE_LOG(LogTemp, Warning, TEXT("GameOver update UI"));
	}
	else 
	{
		GridRoot->RecursiveReveal(x,y);
	}*/
	GridRoot->RecursiveReveal(x, y);
}

void AMinesweeperGameModeBase::MarkTileAsMine(int x, int y, bool MarkAsMine)
{
	ATile* Tile = GridRoot->GetTile(x, y);
	if (MarkAsMine)
	{
		Tile->Mesh->SetMaterial(0, GridRoot->FlagUMaterial);
		Tile->Marked = true;
	}
	else
	{
		Tile->Mesh->SetMaterial(0, GridRoot->EmptyUMaterial);
		Tile->Marked = false;
	}
}

void AMinesweeperGameModeBase::ResetGrid()
{
	GridRoot->ResetGrid();
}

void AMinesweeperGameModeBase::PlayBlastSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(),BlastSound);
}
