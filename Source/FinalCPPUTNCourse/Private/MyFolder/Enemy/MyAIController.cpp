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
	bStartAILogicOnPossess = true;

	//Configuramos los BTs
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Game/0/Enemy/AI/MyBehaviourTree"));
	if (BTAsset.Succeeded())
	{
		behaviorTree = BTAsset.Object;
	}


	ConstructorHelpers::FObjectFinder<UBehaviorTree> ShooterBTAsset(TEXT("/Game/0/Enemy/AI/ShooterBT"));
	if (ShooterBTAsset.Succeeded())
	{
		shooterBT = ShooterBTAsset.Object;
	}


	ConstructorHelpers::FObjectFinder<UBehaviorTree> LongRangeBTAsset(TEXT("/Game/0/Enemy/AI/LongRangeBT"));
	if (LongRangeBTAsset.Succeeded())
	{
		longRangeBT = LongRangeBTAsset.Object;
	}

	//Configuramos la percepci�n del enemigo
	perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	sight->SightRadius = 1000;
	sight->LoseSightRadius = 1100;
	sight->PeripheralVisionAngleDegrees = 180;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->SetMaxAge(3);
	perception->ConfigureSense(*sight);

	perception->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnStimulus);

	//Terminamos de configurar la perceprci�n del enemigo

}

void AMyAIController::OnStimulus(AActor* Actor, FAIStimulus Stimulus)
{
	gameMode = Cast<AFinalCPPUTNCourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) { return; }

	enemy = Cast<AC_Enemy>(GetPawn());
	if (!enemy){UE_LOG(LogTemp, Error, TEXT("[OnStimulus] El enemy no esta disponible aun."));return;}

	AMyNewCharacter* detectedPlayer = Cast<AMyNewCharacter>(Actor);

	if (detectedPlayer != nullptr)
	{
		//Al detectar al player por est�mulo y si player no tiene valor, le pasamos el valor de detectedPlayer, seteamos un bool para el AlertState
		if (player == nullptr)// y agregamos al enemigo a una lista de enemigos en alerta, para saber si ya son suficientes para matar al jugador.
		{
			player = detectedPlayer; //Para detectar al player y limpiar su referencia una vez perdido de vista
			bPlayerDetected = true; //bool global para usar en otros scripts
			enemy->c_EnemyIsAlerted = true; //bool de C_Enemy para cambiar a animaci�n de strafe
			enemy->bUseControllerRotationYaw = false; //Sacandole el control de rotaci�n a la IA para hacer 
			enemy->GetCharacterMovement()->bOrientRotationToMovement = false; //que la rotaci�n sea en base al jugador y no hacia el punto de destino
			Blackboard->SetValueAsObject("Player", detectedPlayer); //Variable de Blackboard para perseguir al player
			GetBlackboardComponent()->SetValueAsBool("isAlerted", true); //Variable del BB para que est� en alerta

			if (!gameMode->AlertedEnemies.Contains(this))
			{
				gameMode->AlertedEnemies.Add(this); //Lo agregamos a la lista de enemigos alertados del GameMode si es que ya no estaba agregado
				if (GetPawn())
				{
					//UE_LOG(LogTemp, Warning, TEXT("Agregado a la lista: %s"), *GetPawn()->GetActorLabel());
				}
			}

		}
		//Si perdemos al jugador, lo volvemos nulo, lo sacamos de la detecci�n booleana y lo sacamos de la lista.
		else if (player == detectedPlayer)
		{
			player = nullptr; //Limpio referencia de detectedPlayer
			bPlayerDetected = false; //bool global para usar en otros scripts
			enemy->c_EnemyIsAlerted = false; //bool de C_Enemy para cambiar a animaci�n de strafe
			enemy->bUseControllerRotationYaw = true; //Le devuelvo el control de rotaci�n a la IA para que 
			enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //pueda rotar hacia los waypoints y no quede raro
			Blackboard->ClearValue("Player"); //Limpiamos referencia de BB
			GetBlackboardComponent()->SetValueAsBool("isAlerted", false); //Perdimos al jugador asi que el enemigo no deber�a estar alertado

			gameMode->AlertedEnemies.Remove(this); //Lo sacamos de la lista de enemigos alertados del GameMode
			if (GetPawn())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Sacado de la lista: %s"), *GetPawn()->GetActorLabel());
			}
		}
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	enemy = Cast<AC_Enemy>(GetPawn());
	if (enemy == nullptr){UE_LOG(LogTemp, Warning, TEXT("[Alert State] Enemy could not be casted on MyAIControler"));return;}
	if (!InPawn){UE_LOG(LogTemp, Warning, TEXT("[Alert State] Theres no pawn to posses"));return;}

	if (InPawn)
	{
		enemy->bUseControllerRotationYaw = true; //Dando el control de rotaci�n a la IA para hacer 
		enemy->GetCharacterMovement()->bOrientRotationToMovement = true; //que la rotaci�n al principio le pertenezca rote segun la direccion de la IA

		if (behaviorTree == nullptr){UE_LOG(LogTemp, Warning, TEXT("behavior tree null"));return;}

		if (shooterBT == nullptr){UE_LOG(LogTemp, Warning, TEXT("Shooter behavior tree null"));return;}

		if (longRangeBT == nullptr){UE_LOG(LogTemp, Warning, TEXT("LongRange behavior tree null"));return;}

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
					SettingSight(2250);
					RunBehaviorTree(shooterBT);
					UE_LOG(LogTemp, Warning, TEXT("Running Shooter BT"));
				}
				break;

			case E_Enemy::LongRange:
				if (longRangeBT)
				{
					SettingSight(3000);
					RunBehaviorTree(longRangeBT);
					UE_LOG(LogTemp, Warning, TEXT("Running LongRange BT"));
				}
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Could not find BT for this enemy"));
				break;

		}
	}	
}

void AMyAIController::SettingSight(float SightRange) //Reconfiguramos los sight settings para el Shooter Enemy
{
	FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());

	if (!Id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
		return;
	}

	// GetPerception() = AIController->GetPerceptionComponent() //Los GetPerceptionComponent estaban como GetPerecption nada mas, pero los cambie porque GetPerception no era nada
	auto Config = GetPerceptionComponent()->GetSenseConfig(Id);

	if (Config == nullptr)
		return;

	auto ConfigSight = Cast<UAISenseConfig_Sight>(Config);

	// Save original lose range
	float LoseRange = ConfigSight->LoseSightRadius - ConfigSight->SightRadius;

	ConfigSight->SightRadius = SightRange;

	// Apply lose range to new radius of the sight
	ConfigSight->LoseSightRadius = ConfigSight->SightRadius + LoseRange;

	//UE_LOG(LogTemp, Warning, TEXT("Sight Range: %.2"), SightRange);

	GetPerceptionComponent()->RequestStimuliListenerUpdate();
}

void AMyAIController::Tick(float DeltaTime)
{
	if (enemy == nullptr){UE_LOG(LogTemp, Warning, TEXT("enemy null on Tick"));return;}
	if (enemy != nullptr)
	{
		enemy->c_EnemyStrafeRight = enemyStrafeRight;
		enemy->c_EnemyStrafeLeft = enemyStrafeLeft;
		enemy->c_EnemyIsAttacking = enemyAttacking;
		enemy->c_EnemyIsRepositioning = enemyReposition;
	}
}


void AMyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (gameMode)
	{
		gameMode->AlertedEnemies.Remove(this); //Si el enemigo muere lo scamos de la lista
	}
}