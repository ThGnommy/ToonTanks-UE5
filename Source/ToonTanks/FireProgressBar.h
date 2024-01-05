// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FireProgressBar.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API UFireProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float LaunchForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* LaunchForceProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextPercent;

	void ResetTextBlockColor();
	void UpdateTextBlockColor(float DeltaTime);
};
