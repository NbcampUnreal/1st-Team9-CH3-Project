// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "BossPulledObject.generated.h"

class AEdenObjectPool;

UCLASS()
class LOSTINEDEN_API ABossPulledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossPulledObject();
	
	UFUNCTION(BlueprintCallable)
	void ReturnToPool();
	UFUNCTION(BlueprintCallable)
	void OnHit(
		UPrimitiveComponent* HitComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void ActivateMovement();
	UFUNCTION(BlueprintCallable)
	void DeactivateMovement();
	UFUNCTION(BlueprintCallable)
	void OnPulled();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "User")
	USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User|Object Pool")
	TSubclassOf<AActor> ObjectPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User|Effect")
	UNiagaraComponent* Niagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User|Object Pool")
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User|Effect")
	USoundCue* Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User|Projectile")
	int32 DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User|Projectile")
	float LifeSpan;
	FTimerHandle LifeSpanTimerHandle;
};
