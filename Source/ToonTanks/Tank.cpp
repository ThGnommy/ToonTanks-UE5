// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Logging/StructuredLog.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "FireProgressBar.h"

ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());


	LoadWidgetInstance = CreateWidget<UFireProgressBar>(GetWorld(), WidgetFireLoadBar);
	WidgetClassRef = Cast<UFireProgressBar>(WidgetFireLoadBarClass);
	//LoadWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetFireLoadBar);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50, 50, FColor::Emerald);

		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATank::LoadFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	Dead = true;

	if (LoadWidgetInstance)
	{
		LoadWidgetInstance->RemoveFromParent();
	}
}

APlayerController* ATank::GetTankPlayerController() const { return TankPlayerController; }

void ATank::Move(float Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * Speed * DeltaTime;

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * DeltaTime * TurnRate;

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::Fire()
{

	float LaunchForce = 0.f;


	if (LoadWidgetInstance)
	{
		LaunchForce = Cast<UFireProgressBar>(LoadWidgetInstance)->LaunchForce;
		UE_LOG(LogTemp, Warning, TEXT("LaunchForce from Tank: %f"), LaunchForce);
		LoadWidgetInstance->RemoveFromParent();
	}

	Super::Fire(LaunchForce);

}

void ATank::LoadFire()
{
	if (LoadWidgetInstance)
	{
		LoadWidgetInstance->AddToViewport(1);
	}
}
