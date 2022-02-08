// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "MinesweeperGameModeBase.h"

void UGameMenu::NativeConstruct()
{
	Super::NativeConstruct();
	HappySadButton->OnClicked.AddDynamic(this,&UGameMenu::OnClick);
}

void UGameMenu::OnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("HappySadButtonClicked"));
	AMinesweeperGameModeBase* GM = Cast<AMinesweeperGameModeBase>(GetWorld()->GetAuthGameMode());
	GM->ResetGrid();
}
