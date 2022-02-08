// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Tile.h"
#include "Grid.generated.h"

UCLASS()
class MINESWEEPER_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	UPROPERTY()
	USceneComponent* RootComp;

	//UPROPERTY(VisibleAnywhere)
	/*UPROPERTY()
	TArray<class UStaticMeshComponent*> TileMeshList;*/

	UPROPERTY()
	TArray<class ATile*> TileMeshList;

	virtual void OnConstruction(const FTransform& Transform) override;

	class ATile* GetTile(int x, int y);
	
	void PlantMines();
	void PopulateNumberTiles();
	void ResetGrid();
	void EmptyTiles();
	int GetNeighbourMineCount(int x, int y);
	void RecursiveReveal(int x, int y);
	void MarkTileAsMine(int x, int y);
	int GetUnclickedTiles();
	bool HasWon();
	void ShowAllMines();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	UMaterial* FlagUMaterial;
	UMaterial* EmptyUMaterial;
	UMaterial* EmptyClickedUMaterial;
	UMaterial* MineUMaterial;
	UMaterial* OneUMaterial;
	UMaterial* TwoUMaterial;
	UMaterial* ThreeUMaterial;
	UMaterial* FourUMaterial;
	UMaterial* FiveUMaterial;
	UMaterial* SixUMaterial;
	UMaterial* SevenUMaterial;
	UMaterial* EightUMaterial;
private:
	int GridLength = 10;
	int CurrentMines = 0;
	int MovesLeft;
	UStaticMesh* SMTile;
};
