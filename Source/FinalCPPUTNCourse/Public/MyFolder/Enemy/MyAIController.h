// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FINALCPPUTNCOURSE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:

	class AMyNewCharacter* player;
	class AC_Enemy* enemy;
	UPROPERTY(EditAnywhere) UAIPerceptionComponent* perception;
	AMyAIController();
	UFUNCTION() void OnStimulus(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBehaviorTree* behaviorTree;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector isAlerted;
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	class AFinalCPPUTNCourseGameMode* gameMode;
	bool bPlayerDetected;
	bool enemyStrafeRight;
	bool enemyStrafeLeft;

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
