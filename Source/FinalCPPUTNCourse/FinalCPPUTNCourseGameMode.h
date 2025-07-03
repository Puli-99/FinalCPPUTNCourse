// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyFolder/Enemy/MyAIController.h"

#include "FinalCPPUTNCourseGameMode.generated.h"

UCLASS(minimalapi)
class AFinalCPPUTNCourseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFinalCPPUTNCourseGameMode();
	UPROPERTY(BlueprintReadWrite) TArray<AMyAIController*> AlertedEnemies;
};



