// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Lab6/Pickup.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/TimerManager.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SetRootComponent(SpawnVolume);

	MinMaxSpawnDelay.Set(1.0f, 4.5f);
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawnVolume::SetSpawning(bool ShouldSpawn) {
	if (ShouldSpawn) 
	{
		SpawnDelay = FMath::FRandRange(MinMaxSpawnDelay.X, MinMaxSpawnDelay.Y);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void ASpawnVolume::SpawnPickup()
{
	if (SpawnItem) 
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation = GetRandomPointInVolume();

			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			APickup* const SpawnedPickup = World->SpawnActor<APickup>(SpawnItem, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(MinMaxSpawnDelay.X, MinMaxSpawnDelay.Y);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

