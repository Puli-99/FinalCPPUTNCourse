// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Player/MyNewCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AMyNewCharacter::AMyNewCharacter()
{
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->bAutoRegister = true;
	//UE_LOG(LogTemp, Warning, TEXT("Player Health: %d"), health);
}


void AMyNewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugCircle(
		GetWorld(),
		GetActorLocation(),          // Centro
		1000,                // Radio
		32,                      // Segmentos (más = más suave)
		FColor::Red,
		false,                   // Persistente (false = desaparece después de un tiempo)
		0.1f,                    // Tiempo de duración (segundos)
		0,                       // Depth Priority
		2.0f,                    // Grosor
		FVector(1, 0, 0),        // Eje X
		FVector(0, 1, 0),        // Eje Y
		false                    // Dibujo en 2D (falso = se adapta al mundo 3D)
	);
}
void AMyNewCharacter::TakeDamage(int damage)
{
	health -= damage;
	//UE_LOG(LogTemp, Warning, TEXT("Player Health: %d"), health);
	Die();
}

void AMyNewCharacter::Die()
{
	if (health <= 0)
	{
		Destroy();
	}
}
