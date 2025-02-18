// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Usable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOSTINEDEN_API IUsable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//모든 아이템이 구현해야할 함수
	virtual void Use() = 0;
};
