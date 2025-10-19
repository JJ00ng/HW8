#include "MyGameMode.h"
#include "MyPawn.h"
#include "MyPlayerController.h"
#include "MyGameState.h"

AMyGameMode::AMyGameMode()
{
    PlayerControllerClass = AMyPlayerController::StaticClass();
    DefaultPawnClass = AMyPawn::StaticClass();
    GameStateClass = AMyGameState::StaticClass();
}