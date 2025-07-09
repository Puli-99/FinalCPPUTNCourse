// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyFolder/Enemy/MyAIController.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotift_Shooting.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API UAnimNotift_Shooting : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	void Shooting(AMyAIController* myController);

};
