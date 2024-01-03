// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Turret.h"
#include "ToonTanksPlayerController.h"

void AToonTankGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartGame();
	HandleGameStart();
}

void AToonTankGameMode::HandleGameStart()
{
	TurretsAlive = GetTurretsAlive();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	TankPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	StartGame();

	if (TankPlayerController)
	{
		TankPlayerController->SetPlayerEnableState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			TankPlayerController,
			&AToonTanksPlayerController::SetPlayerEnableState,
			true
		);

		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);

	}
}

int32 AToonTankGameMode::GetTurretsAlive()
{
	TArray<AActor*> Turrets;
	UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(), Turrets);
	return Turrets.Num();
}

void AToonTankGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank && Tank->GetTankPlayerController())
	{
		Tank->HandleDestruction();
		TankPlayerController->SetPlayerEnableState(false);

		// todo: hud "you lose!"
		GameOver(false);
	}
	else if (ATurret* DestroyedTurret = Cast<ATurret>(DeadActor))
	{
		DestroyedTurret->HandleDestruction();

		// todo: hud "you win!"
		--TurretsAlive;
		if (TurretsAlive == 0)
		{
			GameOver(true);
		}
	}
}