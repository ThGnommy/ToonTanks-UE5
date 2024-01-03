// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	MovementComponent->MaxSpeed = 1300.f;
	MovementComponent->InitialSpeed = 1300.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// sfx values
	float RandomVolume = FMath::RandRange(0.5f, 1.f);
	float RandomPitch = FMath::RandRange(0.9f, 1.1f);

	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSfx, GetActorLocation(), RandomVolume, RandomPitch);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	// sfx values
	float RandomVolume = FMath::RandRange(0.5f, 1.f);
	float RandomPitch = FMath::RandRange(0.9f, 1.1f);

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, UDamageType::StaticClass());

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Hit.Location, OtherActor->GetActorRotation());
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSfx, Hit.Location, RandomVolume, RandomPitch);
	Destroy();
}