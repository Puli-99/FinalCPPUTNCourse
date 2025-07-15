// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "InRangeToShoot.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UInRangeToShoot : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere) FBlackboardKeySelector Player;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShouldEscape;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector SafeRange;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShootPosition;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShootRange;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector isChasing;



	class AMyNewCharacter* player;
	class AMyAIController* myController;



protected:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};