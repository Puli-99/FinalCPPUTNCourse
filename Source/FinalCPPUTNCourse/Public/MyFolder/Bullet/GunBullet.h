// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBullet.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AGunBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBullet();
	UPROPERTY(EditAnywhere) int damage;
	UPROPERTY(EditAnywhere) FVector direction;
	UPROPERTY(EditAnywhere) int speed;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
