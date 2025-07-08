// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Heal.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AHeal : public AActor
{
	GENERATED_BODY()
	
public:	
	void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) int healAmount = 10;
};
