// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFolder/Enemy/InRangeToEscape.h"

#include <MyFolder/Player/MyNewCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MyFolder/Enemy/MyAIController.h"
#include <Kismet/GameplayStatics.h>


EBTNodeResult::Type UInRangeToEscape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	myController = Cast<AMyAIController>(OwnerComp.GetOwner());
	if (myController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AlertState] Cannot cast owner AMyAIController type"));
		return EBTNodeResult::Failed;
	}

	player = Cast<AMyNewCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AlertState] Cannot cast MyNewCharacter"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Warning, TEXT("Debo escapar"));

	FVector playerLocation = player->GetActorLocation();
	FVector enemyLocation = myController->GetPawn()->GetActorLocation();

	float distance = FVector::Dist(playerLocation, enemyLocation);

	if (distance > OwnerComp.GetBlackboardComponent()->GetValueAsFloat(EscapeRange.SelectedKeyName)) //Revisando si debe escapar
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldEscape.SelectedKeyName, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldShoot.SelectedKeyName, true);

		myController->enemyReposition = false;
	}

	return EBTNodeResult::Succeeded;
}