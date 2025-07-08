// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	int ammoAmount = 5;

};
