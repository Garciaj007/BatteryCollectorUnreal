// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Lab6GameMode.h"
#include "Lab6Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"

ALab6GameMode::ALab6GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//Base DecayRate
	DecayRate = 0.01f;
}

void ALab6GameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors) {
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);

		if (SpawnVolumeActor)
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
	}

	SetCurrentState(EBatteryPlayState::Playing);

	//Set Score to beat
	ALab6Character* Character = Cast<ALab6Character>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Character)
		PowerToWin = (Character->GetInitialPower()) * 1.25f;

	if (HUDWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
			CurrentWidget->AddToViewport();
	}

}

void ALab6GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALab6Character* Character = Cast<ALab6Character>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Character) {

		if (Character->GetCurrentPower() > PowerToWin)
			SetCurrentState(EBatteryPlayState::Won);
		else if (Character->GetCurrentPower() > 0)
			Character->UpdatePower(-DeltaTime * DecayRate * Character->GetInitialPower());
		else
			SetCurrentState(EBatteryPlayState::GameOver);
	}
}

float ALab6GameMode::GetPowerToWin() const { return PowerToWin; }

EBatteryPlayState ALab6GameMode::GetCurrentState() const { return CurrentState; }

void ALab6GameMode::SetCurrentState(EBatteryPlayState NewState) 
{ 
	CurrentState = NewState; 
	HandleNewState(CurrentState); 
}

void ALab6GameMode::HandleNewState(EBatteryPlayState NewState)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);

	switch (NewState)
	{
		case EBatteryPlayState::Playing:
		{
			for (auto Volume : SpawnVolumeActors) {
				Volume->SetSpawning(true);
			}
		} break;
		case EBatteryPlayState::Won:
		{
			for (auto Volume : SpawnVolumeActors) {
				Volume->SetSpawning(false);
			}
		} break;
		case EBatteryPlayState::GameOver:
		{
			for (auto Volume : SpawnVolumeActors) 
			{
				Volume->SetSpawning(false);
			}

			if (PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}
			
			if (Character) 
			{
				Character->GetMesh()->SetSimulatePhysics(true);
				Character->GetMovementComponent()->MovementState.bCanJump = false;
			}
		} break;
		default:
		{

		} break;
	}
}

