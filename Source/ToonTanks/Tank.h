// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();
	APlayerController* GetTankPlayerController() const;

	bool IsDead() const { return Dead; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnRate = 100.f;

	UPROPERTY()
	bool Dead = false;

	void Move(float Value);
	void Turn(float Value);

	void Fire();
	void LoadFire();

	APlayerController* TankPlayerController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFireProgressBar> WidgetFireLoadBarClass;

	UFireProgressBar* WidgetClassRef;

	UUserWidget* LoadWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFireProgressBar> WidgetFireLoadBar;

};
