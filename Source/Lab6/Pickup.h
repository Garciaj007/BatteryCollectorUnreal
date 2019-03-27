// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class LAB6_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//True when pickup can be used, or false for deactivation
	bool bIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	//Returns State
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	//Allows external clases to modify id pickup is active
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;

};
