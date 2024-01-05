// Fill out your copyright notice in the Description page of Project Settings.


#include "FireProgressBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void UFireProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (LaunchForceProgressBar && TextPercent)
	{
		LaunchForceProgressBar->SetPercent(0.f);
		ResetTextBlockColor();
	}
}

void UFireProgressBar::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (LaunchForceProgressBar && TextPercent)
	{
		float CurrentPercent = LaunchForceProgressBar->GetPercent();
		float NewPercent = FMath::Clamp(CurrentPercent + DeltaTime, 0.f, 1.f);

		LaunchForceProgressBar->SetPercent(NewPercent);
		LaunchForce = NewPercent * 10;

		FNumberFormattingOptions format;
		format.SetMaximumFractionalDigits(0);

		FText text = FText::AsNumber(CurrentPercent * 100, &format);
		TextPercent->SetText(text);


		UpdateTextBlockColor(DeltaTime);

		UE_LOG(LogTemp, Error, TEXT("Percent: %f"), CurrentPercent);
	}
}

void UFireProgressBar::ResetTextBlockColor()
{
	TextPercent->SetColorAndOpacity(FSlateColor(FLinearColor::White));
}

void UFireProgressBar::UpdateTextBlockColor(float DeltaTime)
{
	FLinearColor CurrentColor = TextPercent->GetColorAndOpacity().GetSpecifiedColor();

	CurrentColor.G -= DeltaTime;
	CurrentColor.B -= DeltaTime;

	TextPercent->SetColorAndOpacity(CurrentColor);
}
