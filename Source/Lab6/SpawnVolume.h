// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class LAB6_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Spawning")
		TSubclassOf<class APickup> SpawnItem;

	FTimerHandle SpawnTimer;

	//Min Max Spawn Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	FVector2D MinMaxSpawnDelay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return SpawnVolume; }

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawning(bool ShouldSpawn);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnVolume;

	void SpawnPickup();

	//Current Spawn Delay
	float SpawnDelay;

};
