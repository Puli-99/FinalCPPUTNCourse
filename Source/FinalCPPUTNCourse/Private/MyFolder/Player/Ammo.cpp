// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFolder/Player/Ammo.h"
#include "MyFolder/Player/MyNewCharacter.h"


void AAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMyNewCharacter* player = Cast<AMyNewCharacter>(OtherActor);
	if (OtherActor == player)
	{
		player->AddAmmo(ammoAmount);
		player->hasMagazines = true;
		Destroy();
	}
}