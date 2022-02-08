// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnClick();

	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* HappySadButton;//button for resetting game on loss
};
