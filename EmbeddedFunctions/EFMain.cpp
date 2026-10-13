#include "EmbeddedFunctions.h"
#include <cstdlib>

int main(void) {
	EmbeddedFunctions funcs;
	//funcs.GOpen("192.168.0.120", 23);
	funcs.GOpen("127.0.0.1", 26000);
	funcs.GCommand("AO 0,5.2");
	funcs.GCommand("CB 7; CB 4; CB 2; CB 5");
	system("pause");
	funcs.GClose();
	return 0;
}
