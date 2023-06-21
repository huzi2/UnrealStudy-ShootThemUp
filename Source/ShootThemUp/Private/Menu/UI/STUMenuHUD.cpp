// Shoot Them Up Game. All Rightes Reserved.


#include "Menu/UI/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		USTUBaseWidget* MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->Show();
		}
	}
}
