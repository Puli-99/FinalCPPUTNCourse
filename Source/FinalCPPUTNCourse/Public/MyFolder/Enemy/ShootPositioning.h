// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ShootPositioning.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UShootPositioning : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:

	class AMyNewCharacter* player;
	class AMyAIController* myController;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector Player;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector SafeRange;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShootPosition;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShootRange;

	UPROPERTY(EditAnywhere) FBlackboardKeySelector isChasing;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
