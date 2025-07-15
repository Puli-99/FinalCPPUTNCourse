// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/EnemySpawner.h"
#include "TimerManager.h"
#include "MyFolder/Enemy/MyAIController.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ID: %.2f"), spawnInterval);
	UE_LOG(LogTemp, Warning, TEXT("SI: %.2f"), initialDelay);
	spawnedEnemies = 0;
	GetWorldTimerManager().SetTimer(spawnTimer, this, &AEnemySpawner::SpawnEnemies, spawnInterval, true, initialDelay);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (spawnedEnemies >= Quantity)
	{
		GetWorldTimerManager().ClearTimer(spawnTimer);
	}
}

void AEnemySpawner::SpawnEnemies()
{
	AC_Enemy* Enemy = GetWorld()->SpawnActor<AC_Enemy>(TypeOfEnemy.Get(), GetActorLocation(), GetActorRotation());

	if (Enemy && !Enemy->GetController())
	{
		Enemy->SpawnDefaultController();
	}
	spawnedEnemies++;
}