// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATurret::ATurret()
{
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (TargetIsInRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATurret::CheckFireCondition()
{
	check(Tank);

	if (TargetIsInRange() && !Tank->IsDead())
	{
		Fire();
	}
}

bool ATurret::TargetIsInRange() const
{
	if (Tank)
	{
		float DistanceToTank = FVector::Distance(GetActorLocation(), Tank->GetActorLocation());
		return DistanceToTank <= FireRange ? true : false;
	}

	return false;
}

void ATurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
