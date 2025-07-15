// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/InRangeToShoot.h"

#include <MyFolder/Player/MyNewCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MyFolder/Enemy/MyAIController.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UInRangeToShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (myController->bPlayerDetected == true)
	{

		FVector playerLocation = player->GetActorLocation();
		FVector enemyLocation = myController->GetPawn()->GetActorLocation();

		float distance = FVector::Dist(playerLocation, enemyLocation);

		if (distance < OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SafeRange.SelectedKeyName)) //Revisando si debe escapar
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldEscape.SelectedKeyName, true);
			myController->enemyReposition = true;
		}

		else
		{
			if (distance > OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ShootRange"))
			{
				myController->enemyAttacking = false;
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(isChasing.SelectedKeyName, true);
			}

			else
			{
				myController->enemyAttacking = true;
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(isChasing.SelectedKeyName, false);
				FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(myController->GetPawn()->GetActorLocation(), player->GetActorLocation());
				myController->GetPawn()->SetActorRotation(lookAt);
			}
		}
	}

	if (myController->bPlayerDetected == false)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(isChasing.SelectedKeyName, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldEscape.SelectedKeyName, false);
		myController->enemyAttacking = false;
		myController->enemyReposition = false;
	}

	return EBTNodeResult::Succeeded;
}