// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Command.h"
#include "Tile.generated.h"

enum Type {
	EMPTY,
	MINE,
	NUMBER
};

UCLASS()
class MINESWEEPER_API ATile : public AActor, public Command
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnSelected(AActor* ClickedActor, FKey KeyPressed);

	
	int X;
	int Y;
	Type TileType;
	bool Marked;
	bool Clicked;
	
	void execute(int x, int y) override;
	void undo(int x, int y) override;
private:
	APlayerController* PC;
	class AMinesweeperGameModeBase* GM;
};
