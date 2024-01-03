// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds Effects")
	USoundBase* LaunchSfx;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds Effects")
	USoundBase* ExplosionSfx;
};
