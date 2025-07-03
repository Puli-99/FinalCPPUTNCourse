// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/Enemy.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include <MyFolder/Player/MyNewCharacter.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Move();
	Super::Tick(DeltaTime);
}

void AEnemy::TakeDamage(int damage)
{
	health -= damage;
	Die();
}

void AEnemy::Die()
{
	if (health <= 0)
	{
		Destroy();
	}
}

void AEnemy::GenerateRandomPosition()
{
	float randomX = FMath::FRandRange(xMin, xMax);
	float randomY = FMath::FRandRange(yMin, yMax);
	randomPosition = FVector(randomX, randomY, GetActorLocation().Z);
	isMovingRandomly = true;
}

void AEnemy::Move()
{
	if (!isMovingRandomly || FVector::Dist(GetActorLocation(), randomPosition) < 50.f)
	{
		GenerateRandomPosition();
	}
	float deltaTime = GetWorld()->GetDeltaSeconds();
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, randomPosition, deltaTime, speed);
	SetActorLocation(NewLocation);
}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMyNewCharacter* player = Cast<AMyNewCharacter>(OtherActor);

	if (player != nullptr)
	{

		if (OtherActor == player)
		{
			player->TakeDamage(enemyDamage);
		}
	}
}
