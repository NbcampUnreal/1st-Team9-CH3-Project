#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	BulletLifeSpan = 3.0f;  // 총알이 3초 후 사라짐

	// 총알 메시 생성
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);
	BulletMesh->SetSimulatePhysics(true);
	BulletMesh->SetCollisionProfileName(TEXT("BlockAll"));
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);

	// 총알 이동 컴포넌트 추가
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->InitialSpeed = 3000.0f;
	BulletMovement->MaxSpeed = 3000.0f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->bShouldBounce = false;

	// 기본 데미지 설정
	BulletDamage = 25.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(BulletLifeSpan);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		// 데미지 적용
		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, GetInstigatorController(), this, UDamageType::StaticClass());

		// 이펙트 스폰
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
		}

		// 사운드 재생
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		}

		// 총알 제거
		Destroy();
	}
}
