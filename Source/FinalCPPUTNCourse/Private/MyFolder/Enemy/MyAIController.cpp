// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/MyAIController.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyFolder/Player/MyNewCharacter.h"
#include "FinalCPPUTNCourse/FinalCPPUTNCourseGameMode.h"
#include <Kismet/GameplayStatics.h>

AMyAIController::AMyAIController()
{
	//Configuramos la percepción del enemigo
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Game/0/Enemy/AI/MyBehaviourTree"));
	if (BTAsset.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("BTAsset Succeed!"));
		behaviorTree = BTAsset.Object;
	}

	perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	UAISenseConfig_Sight* sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	sight->SightRadius = 1000;
	sight->LoseSightRadius = 1100;
	sight->PeripheralVisionAngleDegrees = 120;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->SetMaxAge(3);
	perception->ConfigureSense(*sight);

	perception->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnStimulus);
}
void AMyAIController::OnStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	gameMode = Cast<AFinalCPPUTNCourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) { return; }

	AMyNewCharacter* detectedPlayer = Cast<AMyNewCharacter>(Actor);

	if (detectedPlayer != nullptr)
	{
		//Al detectar al player por estímulo y si player no tiene valor, le pasamos el valor de detectedPlayer, seteamos un bool para el AlertState
		if (player == nullptr)// y agregamos al enemigo a una lista de enemigos en alerta, para saber si ya son suficientes para matar al jugador.
		{
			player = detectedPlayer;
			bPlayerDetected = true;
			Blackboard->SetValueAsObject("Player", detectedPlayer);
			GetBlackboardComponent()->SetValueAsBool("isAlerted", true);

			if (!gameMode->AlertedEnemies.Contains(this))
			{
				gameMode->AlertedEnemies.Add(this);
				if (GetPawn())
				{
					UE_LOG(LogTemp, Warning, TEXT("Agregado a la lista: %s"), *GetPawn()->GetActorLabel());

				}
			}

		}
		//Si perdemos al jugador, lo volvemos nulo, lo sacamos de la detección booleana y lo sacamos de la lista.
		else if (player == detectedPlayer)
		{
			player = nullptr;
			bPlayerDetected = false;
			Blackboard->ClearValue("Player");
			GetBlackboardComponent()->SetValueAsBool("isAlerted", false);

			gameMode->AlertedEnemies.Remove(this);
			if (GetPawn())
			{
				UE_LOG(LogTemp, Warning, TEXT("Sacado de la lista: %s"), *GetPawn()->GetActorLabel());

			}
		}
	}
}
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (behaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("behavior tree null"));

		return;
	}
	RunBehaviorTree(behaviorTree);

}

void AMyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (gameMode)
	{
		gameMode->AlertedEnemies.Remove(this);
	}
}
