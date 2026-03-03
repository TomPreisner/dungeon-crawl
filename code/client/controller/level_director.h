/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
//  The Level Director is a high level controller that manages the interaction in a given level
//      It owns the StageManager and the BattleCoordinator. It relays state between the stage and 
//      battle states.
//  It is the Primary interface for interacting with the game and all inputs should be funneled through it
//     and then dispatched as required

#include "stage_manager.h"
#include "battle_coordinator.h"

class LevelDirector {

private:

};
