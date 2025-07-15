// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AC_Enemy> TypeOfEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Quantity = 5;
	int spawnedEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float spawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float initialDelay;
	FTimerHandle spawnTimer;
	void SpawnEnemies();
};