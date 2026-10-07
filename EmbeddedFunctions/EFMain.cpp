#include "EmbeddedFunctions.h"

int main(void) {
	EmbeddedFunctions funcs;
	funcs.GOpen("192.168.0.120", 23);
	funcs.GCommand("AO 0,3");
	funcs.GClose();
	return 0;
}