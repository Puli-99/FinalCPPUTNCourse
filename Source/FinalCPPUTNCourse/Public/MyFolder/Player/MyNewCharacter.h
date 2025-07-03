// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FinalCPPUTNCourse/FinalCPPUTNCourseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "MyNewCharacter.generated.h"

UCLASS()
class FINALCPPUTNCOURSE_API AMyNewCharacter : public AFinalCPPUTNCourseCharacter
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere) int health = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI") UAIPerceptionStimuliSourceComponent* StimuliSource;

	AMyNewCharacter();
	UFUNCTION(BlueprintCallable, Category = "MyCategory") void TakeDamage(int damage);
	void Die();
	virtual void Tick(float DeltaTime) override;

	
};
