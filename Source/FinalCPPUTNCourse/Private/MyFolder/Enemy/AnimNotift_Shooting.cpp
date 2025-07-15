// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/AnimNotift_Shooting.h"

#include "MyFolder/Enemy/MyAIController.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "MyFolder/Player/MyNewCharacter.h"


void UAnimNotift_Shooting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	AController* Controller = Owner->GetInstigatorController();
	if (!Controller) return;

	AMyAIController* myController = Cast<AMyAIController>(Controller);
	if (myController)
	{
		Shooting(myController);
	}
}

void UAnimNotift_Shooting::Shooting(AMyAIController* myController)
{

	if (!myController || !myController->player) return;

	FVector spawnLocation = myController->GetPawn()->GetActorLocation() + myController->GetPawn()->GetActorForwardVector() * 100.f;
	FRotator spawnRotation = myController->GetPawn()->GetActorRotation();

	UWorld* World = myController->GetPawn()->GetWorld();
	World->SpawnActor<AActor>(myController->enemy->bulletToSpawn, spawnLocation, spawnRotation);
	if (myController->enemy->shootSfx)
	{
		UGameplayStatics::SpawnSound2D(this, myController->enemy->shootSfx);
	}
}