// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	float RotationInterpSpeed = 20.f;

	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, DeltaTime, RotationInterpSpeed));
}

void ABasePawn::Fire()
{
	FVector ProjectileLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileRotation = ProjectileSpawnPoint->GetComponentRotation();
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation);
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	// visual / sfx
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	// sfx values
	float RandomVolume = FMath::RandRange(0.5f, 1.f);
	float RandomPitch = FMath::RandRange(0.9f, 1.1f);

	UGameplayStatics::PlaySoundAtLocation(this, DeathSfx, GetActorLocation(), RandomVolume, RandomPitch);

}
