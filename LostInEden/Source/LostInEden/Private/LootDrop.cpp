// Fill out your copyright notice in the Description page of Project Settings.

#include "LootDrop.h"

ALootDrop::ALootDrop()
{
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(Scene);

	ItemDataTable = nullptr;
	LootChance = 50.0f;
}

void ALootDrop::BeginPlay()
{
	Super::BeginPlay();
	// 난수가 LootChance 보다 크면 아이템 가챠
	int CanLoot = FMath::RandRange(0, 100);
	if (CanLoot > LootChance)
	{
		SpawnRandomItem();

	}
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ALootDrop::DestroyActor,
		2.0f,
		false);
}

void ALootDrop::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			SpawnItem(ActualClass);
		}
	}
}
FItemSpawnRow* ALootDrop::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

void ALootDrop::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return;

	GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetActorLocation(),
		FRotator::ZeroRotator
	);
}
void ALootDrop::DestroyActor()
{
	Destroy();
}


