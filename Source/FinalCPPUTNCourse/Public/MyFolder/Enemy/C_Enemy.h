// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

UENUM(BlueprintType)
enum class E_Enemy :uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Shooter UMETA(DisplayName = "Shooter"),
	LongRange UMETA(DisplayName = "LongRange")
};

UCLASS()
class FINALCPPUTNCOURSE_API AC_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Enemy();

	//Variables propias de C_Enemy
	UPROPERTY(EditAnywhere) int health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int enemyDamage;
	bool isPlayerOverlaping;
	class AMyNewCharacter* myplayer;
	float timer = 0.0f;
	FTimerHandle damageTimer;

	//Variables para controlar animaciones
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool c_EnemyStrafeRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool c_EnemyStrafeLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool c_EnemyIsAlerted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool c_EnemyIsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool c_EnemyIsRepositioning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float c_EnemyInRangeToShoot;

	//Variables de cambio de color al recibir daño
	UPROPERTY()	UMaterialInstanceDynamic* DynamicMaterial;
	FTimerHandle resetingColorTimer;

	//Variable para establecer tipo de enemigo y qué tipo de BT debe ejecutar
	UPROPERTY(EditAnywhere) E_Enemy enemyType;
	//Variables para AnimNotift_Shooting
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AActor> bulletToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USoundBase* shootSfx;

	//Variable del BT
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray <AActor*> waypoints;


	void TakeDamage(int damage);
	void DealDamage();
	void Die();
	void ResetingColor();

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
