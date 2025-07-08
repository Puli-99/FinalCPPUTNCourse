// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/MyAIController.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h" //Para el Rotator del enemigo
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyFolder/Player/MyNewCharacter.h"
#include "FinalCPPUTNCourse/FinalCPPUTNCourseGameMode.h"
#include <Kismet/GameplayStatics.h> //Para el casteo del GameMode

AMyAIController::AMyAIController()
{
	//Configuramos los BTs
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Game/0/Enemy/AI/MyBehaviourTree"));
	if (BTAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("BTAsset Succeed!"));
		behaviorTree = BTAsset.Object;
	}


	ConstructorHelpers::FObjectFinder<UBehaviorTree> ShooterBTAsset(TEXT("/Game/0/Enemy/AI/ShooterBT"));
	if (ShooterBTAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Shooter BTAsset Succeed!"));
		shooterBT = ShooterBTAsset.Object;
	}


	//Configuramos la percepción del enemigo

	perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	sight->SightRadius = 1000;
	sight->LoseSightRadius = 1100;
	sight->PeripheralVisionAngleDegrees = 180;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->SetMaxAge(3);
	perception->ConfigureSense(*sight);

	perception->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnStimulus);

	//Terminamos de configurar la perceprción del enemigo
}
void AMyAIController::OnStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	gameMode = Cast<AFinalCPPUTNCourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) { return; }

	if (!enemy)
	{
		UE_LOG(LogTemp, Error, TEXT("[OnStimulus] El enemy no está disponible aún."));
		return;
	}

	AMyNewCharacter* detectedPlayer = Cast<AMyNewCharacter>(Actor);

	if (detectedPlayer != nullptr)
	{
		//Al detectar al player por estímulo y si player no tiene valor, le pasamos el valor de detectedPlayer, seteamos un bool para el AlertState
		if (player == nullptr)// y agregamos al enemigo a una lista de enemigos en alerta, para saber si ya son suficientes para matar al jugador.
		{
			player = detectedPlayer; //Para detectar al player y limpiar su referencia una vez perdido de vista
			bPlayerDetected = true; //bool global para usar en otros scripts
			enemy->c_EnemyIsAlerted = true; //bool de C_Enemy para cambiar a animación de strafe
			enemy->bUseControllerRotationYaw = false; //Sacandole el control de rotación a la IA para hacer 
			enemy->GetCharacterMovement()->bOrientRotationToMovement = false; //que la rotación sea en base al jugador y no hacia el punto de destino
			Blackboard->SetValueAsObject("Player", detectedPlayer); //Variable de Blackboard para perseguir al player
			GetBlackboardComponent()->SetValueAsBool("isAlerted", true); //Variable del BB para que esté en alerta

			if (!gameMode->AlertedEnemies.Contains(this))
			{
				gameMode->AlertedEnemies.Add(this); //Lo agregamos a la lista de enemigos alertados del GameMode si es que ya no estaba agregado
				if (GetPawn())
				{
					//UE_LOG(LogTemp, Warning, TEXT("Agregado a la lista: %s"), *GetPawn()->GetActorLabel());
				}
			}

		}
		//Si perdemos al jugador, lo volvemos nulo, lo sacamos de la detección booleana y lo sacamos de la lista.
		else if (player == detectedPlayer)
		{
			player = nullptr; //Limpio referencia de detectedPlayer
			bPlayerDetected = false; //bool global para usar en otros scripts
			enemy->c_EnemyIsAlerted = false; //bool de C_Enemy para cambiar a animación de strafe
			enemy->bUseControllerRotationYaw = true; //Le devuelvo el control de rotación a la IA para que 
			enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //pueda rotar hacia los waypoints y no quede raro
			Blackboard->ClearValue("Player"); //Limpiamos referencia de BB
			GetBlackboardComponent()->SetValueAsBool("isAlerted", false); //Perdimos al jugador asi que el enemigo no debería estar alertado

			gameMode->AlertedEnemies.Remove(this); //Lo sacamos de la lista de enemigos alertados del GameMode
			if (GetPawn())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Sacado de la lista: %s"), *GetPawn()->GetActorLabel());

			}
		}
	}
}
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	enemy = Cast<AC_Enemy>(GetPawn());
	if (enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Alert State] Enemy could not be casted on MyAIControler"));
	}
	enemy->bUseControllerRotationYaw = true; //Dando el control de rotación a la IA para hacer 
	enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //que la rotación al principio le pertenezca rote segun la direccion de la IA

	if (behaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("behavior tree null"));

		return;
	}

	if (shooterBT == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("behavior tree null"));

		return;
	}

	switch (enemy->enemyType)
	{
		case E_Enemy::Melee:
			if (behaviorTree)
			{
				RunBehaviorTree(behaviorTree);
				UE_LOG(LogTemp, Warning, TEXT("Running Melee BT"));
			}
			break;

		case E_Enemy::Shooter:
			if (shooterBT)
			{
				RunBehaviorTree(shooterBT);
				sight->SightRadius = 3500;
				sight->LoseSightRadius = 3600;
				sight->PeripheralVisionAngleDegrees = 120;
				UE_LOG(LogTemp, Warning, TEXT("Running Shooter BT"));
			}
			break;
	}

}

void AMyAIController::Tick(float DeltaTime)
{
	enemy->c_EnemyStrafeRight = enemyStrafeRight;
	enemy->c_EnemyStrafeLeft = enemyStrafeLeft;
	enemy->c_EnemyIsAttacking = enemyAttacking;

	//timer += DeltaTime;

	//if (timer < 0.01f)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s %s Atacando"), *enemy->GetActorLabel(), enemyAttacking ? TEXT("esta") : TEXT("no esta"));
	//}

	//if (timer > 0.5f)
	//{
	//	timer = 0.0f;
	//}
}


void AMyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (gameMode)
	{
		gameMode->AlertedEnemies.Remove(this); //Si el enemigo muere lo scamos de la lista
	}
}
