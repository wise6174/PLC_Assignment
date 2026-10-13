//#include "Galil.h"
//
//int main(void) {
//	try {
//		EmbeddedFunctions funcs(true);
//		Galil myGalil(&funcs, "192.168.0.120 -d");
//
//		std::cout << myGalil;
//
//		myGalil.WriteEncoder();
//		std::cout << "Encoder: " << myGalil.ReadEncoder() << std::endl;
//
//		uint16_t value = 255;
//		myGalil.DigitalOutput(value);
//
//		uint16_t newval = myGalil.DigitalInput();
//		myGalil.DigitalByteOutput(false, 3);
//
//		newval = myGalil.DigitalInput();
//		myGalil.DigitalBitOutput(true, 13);
//
//		newval =  myGalil.DigitalInput();
//
//		myGalil.AnalogOutput(0, 5.2);
//
//		myGalil.WriteEncoder();
//
//		int setpoint = 13;
//		std::cout << myGalil.getSetPoint() << std::endl;
//		myGalil.setSetPoint(setpoint);
//		std::cout << myGalil.getSetPoint() << std::endl;
//		myGalil.setKp(2);
//		myGalil.setKi(3);
//		myGalil.SpeedControl(true, 1);
//
//		myGalil.AnalogOutput(0, 0);
//		myGalil.AnalogOutput(0, 0.0);
//		myGalil.AnalogOutput(1, 0.0);
//		myGalil.AnalogOutput(2, 0.0);
//		myGalil.DigitalOutput(0);
//
//		myGalil.WriteEncoder();
//
//		std::cout << newval;
//
//		std::cout << "Command sent successfully.\n";
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Fatal Error: " << e.what() << '\n';
//	}
//
//	std::cin.get(); 
//
//	return 0;
// }

#include "Galil.h"
#include <iostream>

int main() {
	try {
		// 1. Initialize Connection
		EmbeddedFunctions funcs(true);
		Galil myGalil(&funcs, "192.168.0.120 -d");

		std::cout << "--- Testing Operator Overload ---\n";
		std::cout << myGalil;

		// 2. Test Encoder
		std::cout << "--- Testing Encoder ---\n";
		myGalil.WriteEncoder();
		std::cout << "Encoder Initialized to: " << myGalil.ReadEncoder() << "\n\n";

		// 3. Test Digital I/O
		std::cout << "--- Testing Digital I/O ---\n";
		myGalil.DigitalOutput(255);
		uint16_t newval = myGalil.DigitalInput();
		std::cout << "Input after 16-bit Output: " << newval << '\n';

		myGalil.DigitalByteOutput(false, 3);
		newval = myGalil.DigitalInput();
		std::cout << "Input after Byte Output:   " << newval << '\n';

		myGalil.DigitalBitOutput(true, 13);
		newval = myGalil.DigitalInput();
		std::cout << "Input after Bit Output:    " << newval << "\n\n";

		// 4. Test Analog I/O
		std::cout << "--- Testing Analog I/O ---\n";
		myGalil.AnalogOutput(0, 5.2);
		std::cout << "Analog Channel 0 set to 5.2V\n\n";
		myGalil.AnalogOutput(0, 0);
		std::cout << "Encoder currently at " << myGalil.ReadEncoder() << "\n\n";

		// 5. Test Control Loop
		std::cout << "--- Testing Control Loop ---\n";
		//myGalil.WriteEncoder(); // Reset encoder for a clean loop

		int setpoint = 500;
		std::cout << "Initial SetPoint: " << myGalil.getSetPoint() << '\n';

		myGalil.setSetPoint(setpoint);
		std::cout << "Updated SetPoint: " << myGalil.getSetPoint() << '\n';

		myGalil.setKp(0.0005);
		myGalil.setKi(0.0);
		myGalil.setKd(0.0);

		std::cout << "Executing Position Control...\n";
		myGalil.PositionControl(true, 0);

		// 6. Cleanup & Reset hardware states
		std::cout << "\n--- Resetting Simulator States ---\n";
		myGalil.AnalogOutput(0, 0.0);
		myGalil.AnalogOutput(1, 0.0);
		myGalil.AnalogOutput(2, 0.0);
		myGalil.DigitalOutput(0);

		double currEncoder = myGalil.ReadEncoder();
		while (abs(myGalil.ReadEncoder() - currEncoder) > 0) {
			currEncoder = myGalil.ReadEncoder();
		}
		myGalil.WriteEncoder();
		

		std::cout << "\nAll commands executed successfully.\n";
	}
	catch (const std::exception& e) {
		std::cerr << "\nFatal Error: " << e.what() << '\n';
	}

	std::cout << "Press Enter to exit...";
	std::cin.get();

	return 0;
}