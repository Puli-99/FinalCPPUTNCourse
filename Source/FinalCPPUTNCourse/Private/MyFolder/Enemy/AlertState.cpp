// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/AlertState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MyFolder/Enemy/MyAIController.h"
#include <MyFolder/Player/MyNewCharacter.h>
#include "FinalCPPUTNCourse/FinalCPPUTNCourseGameMode.h"
#include "NavigationSystem.h"
#include <Kismet/GameplayStatics.h>



EBTNodeResult::Type UAlertState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	myController = Cast<AMyAIController>(OwnerComp.GetOwner());
	if(myController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AlertState] Cannot cast owner AMyAIController type"));
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[AlertState] Succesfull Cast to My Controller"));

	player = Cast<AMyNewCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AlertState] Cannot cast MyNewCharacter"));
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[AlertState] Succesfull Cast MyNewCharacter"));

	AFinalCPPUTNCourseGameMode* gameMode = Cast<AFinalCPPUTNCourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AlertState] Cannot cast Game Mode"));
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Warning, TEXT("[AlertState] Succesfull Cast to Game Mode"));

	////Setenado movimiento perimetral/de alerta contra jugador
	FVector playerLocation = player->GetActorLocation();

	FVector directionToPlayer = playerLocation - myController->GetPawn()->GetActorLocation();
	directionToPlayer.Z = 0.f; // ignorar altura
	directionToPlayer.Normalize();

	// Generás un RightVector (perpendicular) para moverte lateralmente
	FVector rightVector = FVector::CrossProduct(FVector::UpVector, directionToPlayer);

	// Elegís izquierda o derecha al azar
	float sign = FMath::RandBool() ? 1.f : -1.f;
	if (sign == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));
		myController->enemyStrafeRight = true;
		myController->enemyStrafeLeft = false;
	}
	if (sign == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("-1"));
		myController->enemyStrafeLeft = true;
		myController->enemyStrafeRight = false;
	}


	// Elegís ángulo de dispersión (por ejemplo ±45°)
	float angleDegrees = FMath::RandRange(120.f, 180.f); // cuán lejos del jugador querés estar
	float angleRadians = FMath::DegreesToRadians(angleDegrees);

	// Usás rotación alrededor del jugador
	FVector rotatedDir = directionToPlayer.RotateAngleAxis(sign * angleDegrees, FVector::UpVector);

	// Distancia configurable como ya hacés
	float distance = FMath::RandRange(
		OwnerComp.GetBlackboardComponent()->GetValueAsFloat(minAlertDistance.SelectedKeyName),
		OwnerComp.GetBlackboardComponent()->GetValueAsFloat(maxAlertDistance.SelectedKeyName)
	);

	// Punto final
	FVector perimetralLocation = playerLocation + rotatedDir * distance;
   //OwnerComp.GetBlackboardComponent()->SetValueAsVector(AlertPosition.SelectedKeyName, perimetralLocation);



	FNavLocation navLocation;
	UWorld* world = GEngine->GetWorldFromContextObjectChecked(this);
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	bool isReachable = navSystem && navSystem->ProjectPointToNavigation(perimetralLocation, navLocation, FVector(100.f, 100.f, 100.f));
	UE_LOG(LogTemp, Warning, TEXT("El punto %s alcanzable"), isReachable ? TEXT("es") : TEXT("no es"));

	if (isReachable)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AlertPosition.SelectedKeyName, perimetralLocation);
	}
	else
	{
		// Volver a calcular o log para debug
		UE_LOG(LogTemp, Warning, TEXT("Punto inaccesible, recalculando..."));

		sign = FMath::RandBool() ? 1.f : -1.f;
		if (sign == 1)
		{
			myController->enemyStrafeLeft = false;
			myController->enemyStrafeRight = true;
		}
		if (sign == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("-1"));
			myController->enemyStrafeRight = false;
			myController->enemyStrafeLeft = true;
		}
		angleDegrees = FMath::RandRange(120.f, 180.f); // cuán lejos del jugador querés estar
		angleRadians = FMath::DegreesToRadians(angleDegrees);
		rotatedDir = directionToPlayer.RotateAngleAxis(sign * angleDegrees, FVector::UpVector);

		perimetralLocation = playerLocation + rotatedDir * distance;
		isReachable = navSystem && navSystem->ProjectPointToNavigation(perimetralLocation, navLocation, FVector(100.f, 100.f, 100.f));

		UE_LOG(LogTemp, Warning, TEXT("El nuevo punto %s alcanzable"), isReachable ? TEXT("es") : TEXT("no es"));

		if (isReachable)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AlertPosition.SelectedKeyName, perimetralLocation);
		}

		if (!isReachable)
		{
			UE_LOG(LogTemp, Warning, TEXT("Nada que hacer"));
			return EBTNodeResult::Failed;
		}
	}








	DrawDebugCircle(
		GetWorld(),
		playerLocation,          // Centro
		distance,                // Radio
		32,                      // Segmentos (más = más suave)
		FColor::Green,
		false,                   // Persistente (false = desaparece después de un tiempo)
		5.0f,                    // Tiempo de duración (segundos)
		0,                       // Depth Priority
		2.0f,                    // Grosor
		FVector(1, 0, 0),        // Eje X
		FVector(0, 1, 0),        // Eje Y
		false                    // Dibujo en 2D (falso = se adapta al mundo 3D)
	);

	if (myController->bPlayerDetected == true)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		FVector toPlayer = player->GetActorLocation() - myController->GetPawn()->GetActorLocation();
		toPlayer.Z = 0;

		FRotator lookAtRotation = FRotationMatrix::MakeFromX(toPlayer).Rotator();
		FRotator currentRotation = myController->GetPawn()->GetActorRotation();
		FRotator smoothRotation = FMath::RInterpTo(currentRotation, lookAtRotation, DeltaTime, 5.f);
		myController->GetPawn()->SetActorRotation(smoothRotation);
		UE_LOG(LogTemp, Warning, TEXT("%s En Alerta"), *myController->GetPawn()->GetActorLabel());

		if (gameMode->AlertedEnemies.Num() < 3)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(isAccompanied.SelectedKeyName, false);
			UE_LOG(LogTemp, Warning, TEXT("%s: No somos suficientes"), *myController->GetPawn()->GetActorLabel());
		}

		else if (gameMode->AlertedEnemies.Num() >= 3)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(isAccompanied.SelectedKeyName, true);
			UE_LOG(LogTemp, Warning, TEXT("%s: Ataquemos'"), *myController->GetPawn()->GetActorLabel());
			myController->enemyStrafeRight = false;
			myController->enemyStrafeLeft = false;
		}


		UE_LOG(LogTemp, Warning, TEXT("Enemigos en Alerta: %d"), gameMode->AlertedEnemies.Num());

	}

	else if (myController->bPlayerDetected == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Dejo de estar en alerta"), *myController->GetPawn()->GetActorLabel());
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(isAccompanied.SelectedKeyName, false);
		UE_LOG(LogTemp, Warning, TEXT("Enemigos en Alerta: %d"), gameMode->AlertedEnemies.Num());
		myController->enemyStrafeRight = false;
		myController->enemyStrafeLeft = false;
	}

	return EBTNodeResult::Succeeded;
}