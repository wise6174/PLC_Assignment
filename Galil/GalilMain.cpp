#include "Galil.h"

//int main(void) {
//	try {
//		EmbeddedFunctions funcs(true);
//		Galil myGalil(&funcs, "192.168.0.120 -d");
//
//		uint16_t value = 255;
//		myGalil.DigitalOutput(value);
//		uint16_t newval = myGalil.DigitalInput();
//		myGalil.DigitalByteOutput(false, 3);
//		newval = myGalil.DigitalInput();
//		myGalil.DigitalBitOutput(true, 13);
//		newval =  myGalil.DigitalInput();
//
//		std::cout << newval;
//
//		std::cout << "Command sent successfully.\n";
//	}
//	catch (const std::exception& e) {
//		// This will catch our runtime_errors and print the exact GReturn failure code
//		std::cerr << "Fatal Error: " << e.what() << '\n';
//	}
//
//	std::cin.get(); // Keeps console open to read the result
//	return 0;
// }
int main(void) {
	try {
		std::cout << "Connecting to Galil Simulator...\n";
		EmbeddedFunctions funcs(true);
		Galil myGalil(&funcs, "192.168.0.120 -d");
		std::cout << "Connected successfully.\n\n";

		// --- 1. Test 16-Bit I/O ---
		std::cout << "--- Testing 16-Bit Output/Input ---\n";
		uint16_t out_16 = 43690; // Binary: 1010101010101010
		std::cout << "Writing 16-bit output: " << out_16 << "\n";
		myGalil.DigitalOutput(out_16);

		uint16_t in_16 = myGalil.DigitalInput();
		std::cout << "Read 16-bit input: " << in_16 << "\n\n";


		// --- 2. Test 8-Bit (Byte) I/O on Bank 0 (Low Byte) ---
		std::cout << "--- Testing Byte Output/Input (Bank 0) ---\n";
		uint8_t out_bank0 = 255; // All 8 bits HIGH
		std::cout << "Writing 255 to Bank 0 (Pins 0-7)...\n";
		myGalil.DigitalByteOutput(0, out_bank0);

		// Note: Casting uint8_t to int for cout so it prints as a number, not a char
		uint8_t in_bank0 = myGalil.DigitalByteInput(0);
		std::cout << "Read Bank 0 input: " << static_cast<uint16_t>(in_bank0) << "\n\n";


		// --- 3. Test 8-Bit (Byte) I/O on Bank 1 (High Byte) ---
		std::cout << "--- Testing Byte Output/Input (Bank 1) ---\n";
		uint8_t out_bank1 = 170; // Binary: 10101010
		std::cout << "Writing 170 to Bank 1 (Pins 8-15)...\n";
		myGalil.DigitalByteOutput(1, out_bank1);

		uint8_t in_bank1 = myGalil.DigitalByteInput(1);
		std::cout << "Read Bank 1 input: " << static_cast<uint16_t>(in_bank1) << "\n\n";


		// --- 4. Test Single Bit I/O ---
		std::cout << "--- Testing Single Bit Output/Input ---\n";
		uint8_t test_pin = 4;
		std::cout << "Setting Pin " << static_cast<uint8_t>(test_pin) << " to HIGH (1)...\n";
		myGalil.DigitalBitOutput(true, test_pin);

		bool bit_status = myGalil.DigitalBitInput(test_pin);
		std::cout << "Read Pin " << static_cast<uint8_t>(test_pin) << " status: "
			<< (bit_status ? "HIGH (1)" : "LOW (0)") << "\n\n";


		std::cout << "All tests completed without throwing exceptions!\n";

	}
	catch (const std::exception& e) {
		std::cerr << "Fatal Error during testing: " << e.what() << '\n';
	}

	std::cout << "Press Enter to exit...";
	std::cin.get(); // Keeps the console open to read the results
	return 0;
}