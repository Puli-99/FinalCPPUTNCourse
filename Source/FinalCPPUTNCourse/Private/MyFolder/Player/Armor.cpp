// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Player/Armor.h"
#include "MyFolder/Player/MyNewCharacter.h"

// Sets default values
AArmor::AArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMyNewCharacter* player = Cast<AMyNewCharacter>(OtherActor);
	if (OtherActor == player)
	{
		player->AddArmor(armorAmount);
		Destroy();
	}
}

// Called when the game starts or when spawned
void AArmor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

