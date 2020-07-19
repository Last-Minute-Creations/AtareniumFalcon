#include <ace/generic/main.h>
#include <ace/managers/state.h>

tStateManager *g_pStateMachineGame;
extern tState g_sStateGame;
extern tState g_sStateMenu;

void genericCreate(void) {
	g_pStateMachineGame = stateManagerCreate();
	statePush(g_pStateMachineGame, &g_sStateMenu);
}

void genericProcess(void) {
	stateProcess(g_pStateMachineGame);
}

void genericDestroy(void) {
	stateManagerDestroy(g_pStateMachineGame);
}
