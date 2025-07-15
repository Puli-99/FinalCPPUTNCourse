// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/ShootPositioning.h"

#include <MyFolder/Player/MyNewCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MyFolder/Enemy/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h" //Para el Rotator del enemigo
#include <Kismet/GameplayStatics.h>
#include "MyFolder/Enemy/C_Enemy.h"
#include "Kismet/KismetMathLibrary.h"


EBTNodeResult::Type UShootPositioning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	UE_LOG(LogTemp, Warning, TEXT("%.2f"), distance);
	if (myController->bPlayerDetected == true)
	{

		if (distance < OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ShootRange"))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(isChasing.SelectedKeyName, false);
			return EBTNodeResult::Succeeded;
		}

		else
		{
			myController->enemyAttacking = false;

			myController->enemy->bUseControllerRotationYaw = true; //Dando el control de rotación a la IA para hacer 
			myController->enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //que rote mirando al punto de escape

			FVector directionToPlayer = (playerLocation - enemyLocation).GetSafeNormal();
			float desiredDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ShootRange") - 250.0f;
			FVector shootPosition = playerLocation - directionToPlayer * desiredDistance;

			myController->MoveToLocation(shootPosition);
		}
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(isChasing.SelectedKeyName, false);
		myController->enemyAttacking = false;
		myController->enemyReposition = false;
	}

	return EBTNodeResult::Succeeded;
}