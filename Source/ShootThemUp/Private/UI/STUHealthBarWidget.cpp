// Shoot Them Up Game. All Rightes Reserved.


#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

USTUHealthBarWidget::USTUHealthBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PercentVisibilityThreshold(0.8f)
	, PercentColorThreshold(0.3f)
	, GoodColor(FLinearColor::White)
	, BadColor(FLinearColor::Red)
{
}

void USTUHealthBarWidget::SetHealthPercent(const float Percent)
{
	if (!HealthProgressBar) return;

	const ESlateVisibility HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	HealthProgressBar->SetVisibility(HealthBarVisibility);

	const FLinearColor HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

	HealthProgressBar->SetPercent(Percent);
}
