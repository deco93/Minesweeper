// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "MinesweeperGameModeBase.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	//TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
	SetRootComponent(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForTile(TEXT("/Game/Meshes/Tile"));
	//TODO remove begin
	static ConstructorHelpers::FObjectFinder<UMaterial> FlagMaterial(TEXT("/Game/Materials/Flag_Mat"));
	if (FlagMaterial.Succeeded())
	{
		FlagUMaterial = FlagMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> EmptyMaterial(TEXT("/Game/Materials/Normal_Mat"));
	if (EmptyMaterial.Succeeded())
	{
		EmptyUMaterial = EmptyMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> EmptyClickedMaterial(TEXT("/Game/Materials/Clicked_Mat"));
	if (EmptyClickedMaterial.Succeeded())
	{
		EmptyClickedUMaterial = EmptyClickedMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MineMaterial(TEXT("/Game/Materials/GameOver_Mat"));
	if (MineMaterial.Succeeded())
	{
		MineUMaterial = MineMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> OneMaterial(TEXT("/Game/Materials/1_Mat"));
	if (OneMaterial.Succeeded())
	{
		OneUMaterial = OneMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> TwoMaterial(TEXT("/Game/Materials/2_Mat"));
	if (TwoMaterial.Succeeded())
	{
		TwoUMaterial = TwoMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> ThreeMaterial(TEXT("/Game/Materials/3_Mat"));
	if (ThreeMaterial.Succeeded())
	{
		ThreeUMaterial = ThreeMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> FourMaterial(TEXT("/Game/Materials/4_Mat"));
	if (FourMaterial.Succeeded())
	{
		FourUMaterial = FourMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> FiveMaterial(TEXT("/Game/Materials/5_Mat"));
	if (FiveMaterial.Succeeded())
	{
		FiveUMaterial = FiveMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> SixMaterial(TEXT("/Game/Materials/6_Mat"));
	if (SixMaterial.Succeeded())
	{
		SixUMaterial = SixMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> SevenMaterial(TEXT("/Game/Materials/7_Mat"));
	if (SevenMaterial.Succeeded())
	{
		SevenUMaterial = SevenMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> EightMaterial(TEXT("/Game/Materials/8_Mat"));
	if (EightMaterial.Succeeded())
	{
		EightUMaterial = EightMaterial.Object;
	}
	//EmptyMaterialDyn = UMaterialInstanceDynamic::Create(EmptyUMaterial, this);
	//FlagMaterialDyn = UMaterialInstanceDynamic::Create(FlagUMaterial, this);
	//TODO remove end

	if (MeshForTile.Succeeded())
	{	
		SMTile = MeshForTile.Object;
	}
	
	MovesLeft = (GridLength * GridLength) - GridLength;//cells minus mines
	SetActorLocation(FVector(0, 0, 0));//set grid location at world root/origin
}




void AGrid::OnConstruction(const FTransform& Transform)
{
	float CurrentTileLocationXOffset = -((GridLength * 100) / 2.0f) + 50.0f;//y loc offset
	float CurrentTileLocationYOffset = ((GridLength * 100) / 2.0f) - 50.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//OnClicked.AddDynamic(this, &AGrid::OnSelected);
	for (int x = 0; x < GridLength; x++)
	{
		for (int y = 0; y < GridLength; y++)
		{
			FString TileName = FString("TileMesh") + FString::FromInt(x) + FString::FromInt(y);
			FName TileFName = FName(*TileName);
			
			//UE_LOG(LogTemp, Warning, TEXT("Spawning Tile at location [%f, %f]"), CurrentTileLocationXOffset, CurrentTileLocationYOffset);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), FVector(CurrentTileLocationXOffset, CurrentTileLocationYOffset, 0), FRotator(), SpawnParams);

			UStaticMeshComponent* TileStaticMesh = Cast<UStaticMeshComponent>(Tile->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			USceneComponent* TileRoot = Cast<USceneComponent>(Tile->GetComponentByClass(USceneComponent::StaticClass()));
			Tile->X = x;
			Tile->Y = y;
			Tile->TileType = EMPTY;
			Tile->Marked = false;
			Tile->Clicked = false;

			TileMeshList.Add(Tile);
			
			TileRoot->SetupAttachment(RootComp);
			TileRoot->SetWorldLocation(FVector(CurrentTileLocationXOffset, CurrentTileLocationYOffset, 0));
			if (SMTile)
			{
				TileStaticMesh->SetStaticMesh(SMTile);
				TileStaticMesh->SetMaterial(0, EmptyUMaterial);
			}
			/*TileStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			TileStaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
			TileStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);*/
			CurrentTileLocationXOffset += 100.0f;
		}
		CurrentTileLocationYOffset -= 100.0f;
		CurrentTileLocationXOffset = -((GridLength * 100) / 2.0f) + 50.0f;
	}//end of outer for
	PlantMines();
	PopulateNumberTiles();
	UE_LOG(LogTemp, Warning, TEXT("Length of GridArray [%d]"), TileMeshList.Num());
}

ATile* AGrid::GetTile(int x, int y)
{
	return TileMeshList[(x*GridLength)+y];
}

void AGrid::PlantMines()
{
	CurrentMines = 0;
	while (CurrentMines < GridLength)
	{
		int XRand = FMath::RandRange(0, GridLength - 1);
		int YRand = FMath::FRandRange(0, GridLength - 1);
		ATile* Tile = GetTile(XRand, YRand);
		if (Tile->TileType != MINE)
		{
			Tile->TileType = MINE;
			//Tile->Mesh->SetMaterial(0, MineUMaterial);
			CurrentMines++;
		}
	}
}

void AGrid::PopulateNumberTiles()
{
	for (int x = 0; x < GridLength; x++)
	{
		for (int y = 0; y < GridLength; y++)
		{
			ATile* Tile = GetTile(x, y);
			if (Tile->TileType != MINE)
			{
				int NeighbourMineCount = GetNeighbourMineCount(x, y);
				switch (NeighbourMineCount)
				{
				case 1:
				//Tile->Mesh->SetMaterial(0,OneUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 2:
				//Tile->Mesh->SetMaterial(0, TwoUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 3:
				//Tile->Mesh->SetMaterial(0, ThreeUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 4:
				//Tile->Mesh->SetMaterial(0, FourUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 5:
				//Tile->Mesh->SetMaterial(0, FiveUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 6:
				//Tile->Mesh->SetMaterial(0, SixUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 7:
				//Tile->Mesh->SetMaterial(0, SevenUMaterial);
				Tile->TileType = NUMBER;
				break;
				case 8:
				//Tile->Mesh->SetMaterial(0, EightUMaterial);
				Tile->TileType = NUMBER;
				break;
				default:
				//Tile->Mesh->SetMaterial(0, EmptyUMaterial);
				Tile->TileType = EMPTY;
				}
			}
		}
	}
}

void AGrid::ResetGrid()
{
	MovesLeft = (GridLength * GridLength) - GridLength;
	EmptyTiles();
	PlantMines();
	PopulateNumberTiles();
}

void AGrid::EmptyTiles()
{
	for (int x = 0; x < GridLength; x++)
	{
		for (int y = 0; y < GridLength; y++)
		{
			ATile* Tile = GetTile(x, y);
			Tile->TileType = EMPTY;
			Tile->Marked = false;
			Tile->Clicked = false;
			//UStaticMeshComponent* TileStaticMesh = Cast<UStaticMeshComponent>(Tile->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (SMTile)
			{
				Tile->Mesh->SetStaticMesh(SMTile);
				Tile->Mesh->SetMaterial(0, EmptyUMaterial);
				//TODO set empty material too
			}
		}
	}
	AMinesweeperGameModeBase* GM = Cast<AMinesweeperGameModeBase>(GetWorld()->GetAuthGameMode());
	GM->GameOver = false;
	GM->GameWon = false;
}

int AGrid::GetNeighbourMineCount(int x, int y)
{
	int NeighbourMineCount = 0;
	ATile* TempTile = nullptr;
	//Note tiles index start from bottom right and increase for x leftward and y upward
	if (y + 1 < GridLength && x + 1 < GridLength)//check top left tile
	{
		TempTile = GetTile(x + 1,y + 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (y + 1 < GridLength)//Check top tile
	{
		TempTile = GetTile(x, y + 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (y + 1 < GridLength && x - 1 >= 0)//check top right tile
	{
		TempTile = GetTile(x-1, y + 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (x + 1 < GridLength)//check left tile
	{
		TempTile = GetTile(x + 1, y);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (x - 1 >= 0)//check right tile
	{
		TempTile = GetTile(x - 1, y);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (y - 1 >= 0 && x + 1 < GridLength)//check bottom left tile
	{
		TempTile = GetTile(x + 1, y - 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (y - 1 >= 0 )//check bottom tile
	{
		TempTile = GetTile(x, y - 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	if (y - 1 >= 0 && x - 1 >=0)//check bottom right tile
	{
		TempTile = GetTile(x - 1, y - 1);
		if (TempTile->TileType == MINE)
			NeighbourMineCount++;
	}
	return NeighbourMineCount;
}

void AGrid::RecursiveReveal(int x, int y)
{
	
	if (x < 0 || x >= GridLength || y < 0 || y >= GridLength || MovesLeft==0)
		return;
	ATile* Tile = GetTile(x, y);
	UE_LOG(LogTemp, Warning, TEXT("(%d, %d,    %d"), x, y,Tile->TileType);

	if (Tile->Clicked)
		return;

	if (Tile->TileType == MINE)
	{
		Tile->Mesh->SetMaterial(0, MineUMaterial);
		UE_LOG(LogTemp, Warning, TEXT("GameOver update UI"));
		AMinesweeperGameModeBase* GM = Cast<AMinesweeperGameModeBase>(GetWorld()->GetAuthGameMode());
		GM->GameOver = true;
		ShowAllMines();
		GM->PlayBlastSound();
		return;
	}
	//if (Tile->TileType == MINE || (Tile->TileType == EMPTY && GetNeighbourMineCount(x, y)>0) || Tile->Clicked || Tile->Marked)
	

	if (!Tile->Clicked)
	{
		UE_LOG(LogTemp, Warning, TEXT("(inside tile not clicked"));
		int NeighbourMineCount = GetNeighbourMineCount(x, y);
		Tile->Clicked = true;
		MovesLeft--;
		if (HasWon())
		{
			UE_LOG(LogTemp, Warning, TEXT("Congratulations You Won!"));
			AMinesweeperGameModeBase* GM = Cast<AMinesweeperGameModeBase>(GetWorld()->GetAuthGameMode());
			GM->GameWon = true;
		}
		switch (NeighbourMineCount)
		{
		case 1:
			Tile->Mesh->SetMaterial(0, OneUMaterial);
			return;
		case 2:
			Tile->Mesh->SetMaterial(0, TwoUMaterial);
			return;
		case 3:
			Tile->Mesh->SetMaterial(0, ThreeUMaterial);
			return;
		case 4:
			Tile->Mesh->SetMaterial(0, FourUMaterial);
			return;
		case 5:
			Tile->Mesh->SetMaterial(0, FiveUMaterial);
			return;
		case 6:
			Tile->Mesh->SetMaterial(0, SixUMaterial);
			return;
		case 7:
			Tile->Mesh->SetMaterial(0, SevenUMaterial);
			return;
		case 8:
			Tile->Mesh->SetMaterial(0, EightUMaterial);
			return;
		default:
			Tile->Mesh->SetMaterial(0, EmptyClickedUMaterial);
		}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("MovesLeft %d"), MovesLeft);
	if(x+1 < GridLength && y+1 < GridLength && GetTile(x+1, y+1)->TileType != MINE)
		RecursiveReveal(x + 1, y + 1);//reveal top left

	if(y+1< GridLength && GetTile(x, y + 1)->TileType != MINE)
		RecursiveReveal(x, y + 1);//reveal top

	if(x-1>=0 && y+1< GridLength && GetTile(x - 1, y + 1)->TileType != MINE)
		RecursiveReveal(x - 1, y + 1);//reveal top right

	if(x+1<GridLength && GetTile(x + 1, y)->TileType != MINE)
		RecursiveReveal(x + 1, y);//reveal left

	if(x-1>=0 && GetTile(x - 1, y)->TileType != MINE)
		RecursiveReveal(x - 1, y);//reveal right

	if(x+1<GridLength && y-1>=0 && GetTile(x + 1, y - 1)->TileType != MINE)
		RecursiveReveal(x + 1, y - 1);//reveal bottom left

	if(y-1>=0 && GetTile(x, y - 1)->TileType != MINE)
		RecursiveReveal(x, y - 1);//reveal bottom

	if(x-1>=0 && y-1>=0 && GetTile(x - 1, y - 1)->TileType != MINE)
		RecursiveReveal(x - 1, y - 1);//reveal bottom right

	/*if (HasWon())
	{
		UE_LOG(LogTemp, Warning, TEXT("Congratulations You Won!"));
	}*/
	/*if (!Tile->Clicked )
	{
		UE_LOG(LogTemp, Warning, TEXT("(inside tile not clicked"));
		int NeighbourMineCount = GetNeighbourMineCount(x, y);
		Tile->Clicked = true;
		MovesLeft--;
		switch (NeighbourMineCount)
		{
		case 1:
			Tile->Mesh->SetMaterial(0, OneUMaterial);
			break;
		case 2:
			Tile->Mesh->SetMaterial(0, TwoUMaterial);
			break;
		case 3:
			Tile->Mesh->SetMaterial(0, ThreeUMaterial);
			break;
		case 4:
			Tile->Mesh->SetMaterial(0, FourUMaterial);
			break;
		case 5:
			Tile->Mesh->SetMaterial(0, FiveUMaterial);
			break;
		case 6:
			Tile->Mesh->SetMaterial(0, SixUMaterial);
			break;
		case 7:
			Tile->Mesh->SetMaterial(0, SevenUMaterial);
			break;
		case 8:
			Tile->Mesh->SetMaterial(0, EightUMaterial);
			break;
		default:
			Tile->Mesh->SetMaterial(0, EmptyUMaterial);
		}
	}*/
}

void AGrid::MarkTileAsMine(int x, int y)
{
	ATile* Tile = GetTile(x, y);
	Tile->Mesh->SetMaterial(0, FlagUMaterial);
}
int AGrid::GetUnclickedTiles()
{
	int UnClickedTiles=0;
	for (int x = 0; x < GridLength; x++)
	{
		for (int y = 0; y < GridLength; y++)
		{
			if (!GetTile(x, y)->Clicked)
				UnClickedTiles++;
		}
	}
	return UnClickedTiles;
}

bool AGrid::HasWon()
{
	bool WinCondition = false;
	if (GetUnclickedTiles() == GridLength) 
	{
		WinCondition = true;
	}
	return WinCondition;
}

void AGrid::ShowAllMines()
{
	for (int x = 0; x < GridLength; x++)
	{
		for (int y = 0; y < GridLength; y++)
		{
			ATile* Tile = GetTile(x, y);
			if (!Tile->Clicked && Tile->TileType == MINE)
			{
				Tile->Mesh->SetMaterial(0, MineUMaterial);
				Tile->Clicked = true;
			}

		}
	}
}
// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}