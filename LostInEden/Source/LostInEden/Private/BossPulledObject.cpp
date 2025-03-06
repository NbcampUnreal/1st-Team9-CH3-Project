// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPulledObject.h"
#include "EdenObjectPool.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABossPulledObject::ABossPulledObject()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	SetRootComponent(CollisionSphere);
	CollisionSphere->SetEnableGravity(false);
	CollisionSphere->SetSimulatePhysics(false);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Niagara->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(CollisionSphere);
	ProjectileMovement->InitialSpeed = 600.0f;
	ProjectileMovement->MaxSpeed = 600.0f;
	ProjectileMovement->Friction = 0.0f;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	CollisionSphere->OnComponentHit.AddDynamic(this, &ABossPulledObject::OnHit);

	LifeSpan = 10.0f;
	DamageAmount = 10;
}

void ABossPulledObject::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor) //&& OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			DamageAmount, 
			nullptr, 
			this, 
			UDamageType::StaticClass());
		
		// 이펙트 재생 추가
	}
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
	ReturnToPool();
}

void ABossPulledObject::BeginPlay()
{
	Super::BeginPlay();

}

void ABossPulledObject::ReturnToPool()
{
	if (ObjectPool)
	{
		DeactivateMovement();
		Cast<AEdenObjectPool>(ObjectPool)->ReturnPooledObject(this);
	}
}
void ABossPulledObject::OnPulled()
{
	if (ObjectPool)
	{
		AddActorWorldOffset(GetActorForwardVector() * 600.0f);
		ProjectileMovement->Velocity = GetActorForwardVector() * 600.0f;
	}
}

void ABossPulledObject::ActivateMovement()
{
	ProjectileMovement->Activate();
	Niagara->Activate();
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &ABossPulledObject::ReturnToPool, LifeSpan, false);
	
}

void ABossPulledObject::DeactivateMovement()
{
	ProjectileMovement->Deactivate();

	Niagara->DeactivateImmediate();
}


