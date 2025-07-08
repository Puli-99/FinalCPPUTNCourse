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
	

	FVector playerLocation = player->GetActorLocation();
	FVector enemyLocation = myController->GetPawn()->GetActorLocation();

	float distance = FVector::Dist(playerLocation, enemyLocation);

	if (distance > OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SafeRange.SelectedKeyName)) //Revisando si debe escapar
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShouldEscape.SelectedKeyName, true);
	}

	else
	{
		if (myController->bPlayerDetected == true)
		{
			UClass* bulletClass = OwnerComp.GetBlackboardComponent()->GetValueAsClass(EnemyBullet.SelectedKeyName);
			if (!bulletClass)
			{
				UE_LOG(LogTemp, Warning, TEXT("[AlertState] bulletClass is null"));
				return EBTNodeResult::Failed;
			}

			FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(myController->GetPawn()->GetActorLocation(), player->GetActorLocation());
			myController->GetPawn()->SetActorRotation(lookAt);

			FVector spawnLocation = myController->GetPawn()->GetActorLocation() + myController->GetPawn()->GetActorForwardVector() * 100.f;
			FRotator spawnRotation = myController->GetPawn()->GetActorRotation();

			myController->GetPawn()->GetWorld()->SpawnActor<AActor>(bulletClass, spawnLocation, spawnRotation);
		}
		
	}
	return EBTNodeResult::Succeeded;
}