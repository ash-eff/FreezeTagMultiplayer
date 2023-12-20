// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if(DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
		case ROLE_Authority:
			Role = FString("Authority");
			break;
		case ROLE_AutonomousProxy:
			Role = FString("Autonomous Proxy");
			break;
		case ROLE_SimulatedProxy:
			Role = FString("Simulated Proxy");
			break;
		case ROLE_None:
			Role = FString("None");
			break;
		default:
			Role = FString("None");
	}
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	APlayerState* PlayerState = InPawn->GetPlayerState();
	if (PlayerState)
	{
		FString PlayerName = PlayerState->GetPlayerName();
		SetDisplayText(PlayerName);
	}
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
