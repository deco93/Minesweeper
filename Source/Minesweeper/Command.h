// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MINESWEEPER_API Command
{
public:
	virtual ~Command() {}
	virtual void execute(int x, int y) = 0;
	virtual void undo(int x, int y) = 0;
};
