// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AC_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Enemy();

	UPROPERTY(EditAnywhere) int health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int enemyDamage;
	UPROPERTY(EditAnywhere) TArray <AActor*> waypoints;
	bool isPlayerOverlaping;
	//bool isPlayerBeingDamaged;
	class AMyNewCharacter* myplayer;

	//UPROPERTY(EditAnywhere) FTimerHandle damageTimer;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite) float time = 0.2f;

	float timer1 = 0.0f;


	void TakeDamage(int damage);
	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
