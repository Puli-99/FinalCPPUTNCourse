// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	UPROPERTY(EditAnywhere) int health = 100;
	UPROPERTY(EditAnywhere) int speed;
	UPROPERTY(EditAnywhere) int enemyDamage;

	UPROPERTY(EditAnywhere) FVector randomPosition;
	UPROPERTY(EditAnywhere) bool isMovingRandomly;
	UPROPERTY(EditAnywhere) float xMin;
	UPROPERTY(EditAnywhere) float xMax;
	UPROPERTY(EditAnywhere) float yMin;
	UPROPERTY(EditAnywhere) float yMax;


	void TakeDamage(int damage);
	void Die();
	void Move();
	void GenerateRandomPosition();
	void NotifyActorBeginOverlap(AActor* OtherActor) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
