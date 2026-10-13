#include "Galil.h"

int main(void) {
	try {
		EmbeddedFunctions funcs(true);
		Galil myGalil(&funcs, "192.168.0.120 -d");

		//std::cout << myGalil;

		myGalil.WriteEncoder();
		std::cout << "Encoder: " << myGalil.ReadEncoder() << std::endl;

		uint16_t value = 255;
		myGalil.DigitalOutput(value);

		uint16_t newval = myGalil.DigitalInput();
		myGalil.DigitalByteOutput(false, 3);

		newval = myGalil.DigitalInput();
		myGalil.DigitalBitOutput(true, 13);

		newval =  myGalil.DigitalInput();

		myGalil.AnalogOutput(0, 5.2);

		myGalil.WriteEncoder();

		int setpoint = 13;
		std::cout << myGalil.getSetPoint() << std::endl;
		myGalil.setSetPoint(setpoint);
		std::cout << myGalil.getSetPoint() << std::endl;
		myGalil.setKp(2);
		myGalil.setKi(3);
		myGalil.SpeedControl(true, 1);

		myGalil.AnalogOutput(0, 0);
		myGalil.AnalogOutput(0, 0.0);
		myGalil.AnalogOutput(1, 0.0);
		myGalil.AnalogOutput(2, 0.0);
		myGalil.DigitalOutput(0);

		myGalil.WriteEncoder();

		std::cout << newval;

		std::cout << "Command sent successfully.\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << '\n';
	}

	std::cin.get(); 

	return 0;
 }
