// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFolder/Player/Heal.h"
#include "MyFolder/Player/MyNewCharacter.h"

void AHeal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMyNewCharacter* player = Cast<AMyNewCharacter>(OtherActor);
	if (OtherActor == player)
	{
		player->TakeDamage(healAmount);
		Destroy();
	}

}

