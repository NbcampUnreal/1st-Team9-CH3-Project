// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "GlobalEnum.generated.h"


// ���� ���ݿ��� Global�ϰ� ���Ǵ� Enum Class ����


/// <summary>
/// Title Level���� ���Ǵ� Stage ����
/// </summary>
UENUM(BlueprintType)
enum class ETitleStageIndex : uint8
{
	Title		UMETA(DisplayName = "Ÿ��Ʋ"),
	Scenematic	UMETA(DisplayName = "�ó׸�ƽ����")
};

/// <summary>
/// Main Level���� ���Ǵ� Stage ����
/// </summary>
UENUM(BlueprintType)
enum class EEdenStageIndex : uint8
{
	Aisle1		UMETA(DisplayName = "���1"),
	Wave1		UMETA(DisplayName = "���̺�1"),
	Aisle2		UMETA(DisplayName = "���2"),
	Wave2		UMETA(DisplayName = "���̺�2"),
	Aisle3		UMETA(DisplayName = "���3"),
	Boss		UMETA(DisplayName = "����"),
};

/// <summary>
/// Ending Level���� ���Ǵ� Stage ����
/// </summary>
UENUM(BlueprintType)
enum class EEndingStageIndex : uint8
{
	Ending		UMETA(DisplayName = "����"),
};


/*
UCLASS()
class LOSTINEDEN_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
*/