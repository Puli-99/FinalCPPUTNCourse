// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFolder/Enemy/InRangeToEscape.h"

#include <MyFolder/Player/MyNewCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MyFolder/Enemy/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h" //Para el Rotator del enemigo
#include "MyFolder/Enemy/C_Enemy.h"
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


	FVector playerLocation = player->GetActorLocation();
	FVector enemyLocation = myController->GetPawn()->GetActorLocation();

	float distance = FVector::Dist(playerLocation, enemyLocation);

	if (distance > OwnerComp.GetBlackboardComponent()->GetValueAsFloat(EscapeRange.SelectedKeyName)) //Revisando si debe escapar
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldEscape.SelectedKeyName, false);

		myController->enemyReposition = false;
	}

	else
	{
		myController->enemy->bUseControllerRotationYaw = true; //Dando el control de rotación a la IA para hacer 
		myController->enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //que rote mirando al punto de escape
		FVector directionAway = (enemyLocation - playerLocation).GetSafeNormal();
		float fleeDistance = 500.f; // Podés ajustarlo a gusto o convertirlo en variable editable
		FVector fleeDestination = enemyLocation + directionAway * fleeDistance;

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(EscapePosition.SelectedKeyName, fleeDestination);
		myController->MoveToLocation(fleeDestination);
	}

	return EBTNodeResult::Succeeded;
}