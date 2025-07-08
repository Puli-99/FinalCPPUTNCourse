// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Enemy/EnemyBullet.h"
#include "MyFolder/Player/MyNewCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "FinalCPPUTNCourse/FinalCPPUTNCourseCharacter.h"
#include "Camera/CameraComponent.h"
// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	AddActorLocalOffset(direction * speed, true);

	Super::Tick(DeltaTime);
}

void AEnemyBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMyNewCharacter* player = Cast<AMyNewCharacter>(OtherActor);

	if (player != nullptr)
	{

		if (OtherActor == player)
		{
			player->TakeDamage(damage);
		}
	}
	Destroy();
}