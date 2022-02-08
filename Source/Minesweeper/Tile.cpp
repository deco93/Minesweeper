// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "MinesweeperGameModeBase.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp1");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile");
	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForTile(TEXT("/Game/Meshes/Tile"));
	if (MeshForTile.Succeeded())
	{
		Mesh->SetStaticMesh(MeshForTile.Object);
		/*Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);*/
	}
	this->OnClicked.AddDynamic(this, &ATile::OnSelected);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->ClickEventKeys.Add(EKeys::RightMouseButton);
	GM = Cast<AMinesweeperGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::OnSelected(AActor* ClickedActor, FKey KeyPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Tile Clicked (%d,%d)"), X, Y);
	if (!GM->GameOver && !GM->GameWon)
	{
		if (KeyPressed == EKeys::LeftMouseButton)
		{
			UE_LOG(LogTemp, Warning, TEXT("LMB Tile Clicked (%d,%d)"), X, Y);
			AMinesweeperGameModeBase* MinesweeperGameModeBase = (AMinesweeperGameModeBase*)GetWorld()->GetAuthGameMode();
			MinesweeperGameModeBase->RevealTile(X, Y);
		}
		else if (KeyPressed == EKeys::RightMouseButton)
		{
			UE_LOG(LogTemp, Warning, TEXT("RMB Tile Clicked (%d,%d)"), X, Y);
			AMinesweeperGameModeBase* MinesweeperGameModeBase = (AMinesweeperGameModeBase*)GetWorld()->GetAuthGameMode();
			if (!Marked && !Clicked)
				execute(X, Y);
			else if(Marked && !Clicked)
				undo(X, Y);
				//MinesweeperGameModeBase->MarkTileAsMine(X, Y);
		}
	}
}

void ATile::execute(int x, int y)
{
	AMinesweeperGameModeBase* MinesweeperGameModeBase = (AMinesweeperGameModeBase*)GetWorld()->GetAuthGameMode();
	MinesweeperGameModeBase->MarkTileAsMine(X, Y, true);
}

void ATile::undo(int x, int y)
{
	AMinesweeperGameModeBase* MinesweeperGameModeBase = (AMinesweeperGameModeBase*)GetWorld()->GetAuthGameMode();
	MinesweeperGameModeBase->MarkTileAsMine(X, Y, false);
}
