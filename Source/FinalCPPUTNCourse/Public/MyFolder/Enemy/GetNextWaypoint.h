// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GetNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UGetNextWaypoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere) FBlackboardKeySelector waypointIndexKey;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector waypointLocationKey;


protected:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
