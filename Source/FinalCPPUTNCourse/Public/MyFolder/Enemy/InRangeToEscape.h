// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "InRangeToEscape.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UInRangeToEscape : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere) FBlackboardKeySelector Player;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector ShouldEscape;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector SafeRange;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector EscapeRange;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector EscapePosition;

	class AMyNewCharacter* player;
	class AMyAIController* myController;


protected:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
