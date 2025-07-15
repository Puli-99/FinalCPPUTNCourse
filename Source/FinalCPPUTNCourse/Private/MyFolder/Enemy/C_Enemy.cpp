// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/C_Enemy.h"
#include <MyFolder/Player/MyNewCharacter.h>
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values
AC_Enemy::AC_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (AIControllerClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller no encontrado"));
		return;
	}
}

// Called when the game starts or when spawned
void AC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	myplayer = Cast<AMyNewCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (myplayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jugador no encontrado"));
	}
	else if(myplayer != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Se encontro al jugador al iniciar"));
	}

	DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
}

void AC_Enemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//GetWorldTimerManager().SetTimer(damageTimer, this, &AC_Enemy::DealDamage, time, true);
	if (OtherActor == myplayer)
	{
		isPlayerOverlaping = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlapeando Jugador"));
	}
}

void AC_Enemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	//GetWorldTimerManager().ClearTimer(damageTimer);
	if (OtherActor == myplayer)
	{
		isPlayerOverlaping = false;
		UE_LOG(LogTemp, Warning, TEXT("Se termino el overlapeado"));
	}
}

// Called every frame
void AC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isPlayerOverlaping)
	{
		timer += DeltaTime;

		if (timer < 0.01f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s esta atacando"), *GetActorLabel());
			myplayer->TakeDamage(enemyDamage);
		}

	}

	if (timer > 0.2f)
	{
		timer = 0.0f;
	}
	//Reemplazar esto por algo como esto: GetWorldTimerManager().SetTimer(damageTimer, this, myplayer->TakeDamage(enemyDamage), time, true);
	//Preguntar cómo hacer para que se tome myplayer->TakeDamage()
}

// Called to bind functionality to input
void AC_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_Enemy::Die()
{
	if (health <= 0)
	{
		Destroy();
	}
}

void AC_Enemy::TakeDamage(int damage)
{
	health -= damage;
	UE_LOG(LogTemp, Warning, TEXT("%s Health: %d"), *GetActorLabel(), health);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Param", FLinearColor(1.f, 0.f, 0.f, 0.f));
	}
	GetWorldTimerManager().SetTimer(resetingColorTimer, this, &AC_Enemy::ResetingColor, 0.1f, false);
	Die();
}

void AC_Enemy::ResetingColor()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Param", FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
}