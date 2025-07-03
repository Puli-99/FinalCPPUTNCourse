// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/GetNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "MyFolder/Enemy/MyAIController.h"


EBTNodeResult::Type UGetNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyAIController* controller = Cast<AMyAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetNextWaypoint] Cannot cast owner AAIController type"));
		return EBTNodeResult::Failed;
	}

	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());
	if (enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetNextWaypoint] Cannot cast owner to AC_Enemy type"));
		return EBTNodeResult::Failed;
	}

	if (enemy->waypoints.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetNextWaypoint] Enemy doesn't have any waypoints"));
		return EBTNodeResult::Failed;
	}

	int index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(waypointIndexKey.SelectedKeyName);
	FVector waypointPosition = enemy->waypoints[index]->GetActorLocation();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(waypointLocationKey.SelectedKeyName, waypointPosition);

	index++;
	if (index >= enemy->waypoints.Num())
	{
		index = 0;
	}
	if (!enemy->waypoints[index])
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetNextWaypoint] Waypoint at index %d is null"), index);
		return EBTNodeResult::Failed;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(waypointIndexKey.SelectedKeyName, index);

	return EBTNodeResult::Succeeded;
}