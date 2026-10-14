#include "EmbeddedFunctions.h"
int main(void) {
	EmbeddedFunctions^ ef = gcnew EmbeddedFunctions();
	ef->GOpen("192.168.0.120", 26000);
	Threading::Thread::Sleep(50);

	String^ Response;

	// Check sending a command
	Console::WriteLine("Sending command: OP 255, 255;");
	Response = ef->GCommand("OP 255, 255;");
	Console::WriteLine("Response: " + Response);

	// Check receiving a response
	Threading::Thread::Sleep(50);
	Console::WriteLine("Sending command: TI 0;");
	Response = ef->GCommand("TI 0;");
	Console::WriteLine("Response: " + Response);

	Console::WriteLine("Press any key to continue...");
	Console::ReadKey();

	// Set the analog output to 3.0V, without the semicolon (they should check for this)
	ef->GCommand("AO 0, 3.0");
	Threading::Thread::Sleep(1000);
	ef->GCommand("AO 0, 0.0");
	// Read the AI0 voltage (potentiometer) and print it to the console until a key is pressed
	while (!Console::KeyAvailable) {
		Response = ef->GCommand("MG @AN[0];");
		Console::WriteLine("Voltage Response:" + Response);
		Threading::Thread::Sleep(100);
	}

	ef->GClose();
	Console::WriteLine("Press any key to continue...");
	Console::ReadKey();

	return 0;
}