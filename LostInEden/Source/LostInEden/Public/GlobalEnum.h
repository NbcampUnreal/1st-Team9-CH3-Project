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
};

/// <summary>
/// Main Level���� ���Ǵ� Stage ����
/// </summary>
UENUM(BlueprintType)
enum class EEdenStageIndex : uint8
{
	Ground		UMETA(DisplayName = "�������"),
	Basement1	UMETA(DisplayName = "�������1"),
	Wave1		UMETA(DisplayName = "���̺�1"),
	Basement2	UMETA(DisplayName = "�������2"),
	Wave2		UMETA(DisplayName = "���̺�2"),
	Basement3	UMETA(DisplayName = "�������3"),
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
e.g.
UENUM(BlueprintType)
enum class EGlobalEnum : uint8
{
	Stage1	UMETA(DisplayName = "����"),
	Stage2	UMETA(DisplayName = "����"),
};
*/

/*
UCLASS()
class LOSTINEDEN_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
*/