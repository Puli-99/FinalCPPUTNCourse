// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Bullet/GunBullet.h"
#include "MyFolder/Enemy/C_Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "FinalCPPUTNCourse/FinalCPPUTNCourseCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
AGunBullet::AGunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunBullet::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
}

// Called every frame
void AGunBullet::Tick(float DeltaTime)
{
	AddActorLocalOffset(direction * speed, true);
}

void AGunBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy != nullptr)
	{

		if (OtherActor == enemy)
		{
			enemy->TakeDamage(damage);
		}
	}
	Destroy();
}