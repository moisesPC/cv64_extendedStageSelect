#include "unknown_struct.h"
#include "gamestate.h"

void extendedStageSelect_hook() {
    if ((D_80383AB8.controllers[0].buttons_held & BTN_L) &&
        (D_80383AB8.controllers[0].buttons_held & BTN_R) &&
        (D_80383AB8.controllers[0].buttons_held & BTN_Z))
    {
        changeGameState(STAGE_SELECT);
    }
}
