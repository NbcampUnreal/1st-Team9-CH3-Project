// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenObjectPool.h"
#include "BossPulledObject.h"

AEdenObjectPool::AEdenObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
	PoolSize = 300;
}

void AEdenObjectPool::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < PoolSize; i++)
	{
		AActor* PoolObject = GetWorld()->SpawnActor<AActor>(ObjectToPool);
		if (PoolObject)
		{
			Deactivate(PoolObject);
			PooledObjects.Add(PoolObject);
		}
	}
}

AActor* AEdenObjectPool::GetPooledObejct()
{
	for (AActor* Object : PooledObjects)
	{
		if (Object && Object->IsHidden())
		{
			Activate(Object);
			ABossPulledObject* PulledObject = Cast<ABossPulledObject>(Object);
			if (PulledObject)
			{
				PulledObject->ActivateMovement();
			}
			return Object;
		}
		else 
		{
			UE_LOG(LogTemp, Display, TEXT("발사체 부족!"));
		}
	}
	return nullptr;
}
void AEdenObjectPool::ReturnPooledObject(AActor* PooledObject)
{
	if (PooledObject)
	{
		Deactivate(PooledObject);
	}
}

void AEdenObjectPool::Activate(AActor* PooledObject)
{
	PooledObject->SetActorEnableCollision(true);
	PooledObject->SetActorHiddenInGame(false);

}
void AEdenObjectPool::Deactivate(AActor* PooledObject)
{
	PooledObject->SetActorEnableCollision(false);
	PooledObject->SetActorHiddenInGame(true);
	
	PooledObject->SetActorLocation(FVector(0, 0, 0));
	PooledObject->SetActorRotation(FRotator::ZeroRotator);

	ABossPulledObject* Object = Cast<ABossPulledObject>(PooledObject);
	if (Object)
	{
		Object->DeactivateMovement();
	}

}
