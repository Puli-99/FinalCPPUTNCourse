// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AlertState.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UAlertState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere) FBlackboardKeySelector Player;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector AlertPosition;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector minAlertDistance;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector maxAlertDistance;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector isAccompanied;


	class AMyNewCharacter* player;
	class AMyAIController* myController;



protected:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



	
};
