// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Lab6GameMode.generated.h"

//Enum State Gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8 {
	Playing,
	GameOver, 
	Won,
	Unknown
};

UCLASS(minimalapi)
class ALab6GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALab6GameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	void SetCurrentState(EBatteryPlayState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float PowerToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	EBatteryPlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	void HandleNewState(EBatteryPlayState NewState);
};



