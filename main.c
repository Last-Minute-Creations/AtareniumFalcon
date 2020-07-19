#include <ace/managers/state.h>

tStateManager *g_pStateMachineGame;
extern tState g_sStateGame;

void genericCreate(void) {
    g_pStateMachineGame = stateManagerCreate();
    statePush(g_pStateMachineGame, &g_sStateGame);
}

void genericLoop(void) {
   stateProcess(g_pStateMachineGame); 
}


void genericDestroy(void) {
    stateManagerDestroy(g_pStateMachineGame);
}