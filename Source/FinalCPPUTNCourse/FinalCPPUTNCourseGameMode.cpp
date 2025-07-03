// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalCPPUTNCourseGameMode.h"
#include "FinalCPPUTNCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFinalCPPUTNCourseGameMode::AFinalCPPUTNCourseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
