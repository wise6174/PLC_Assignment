#include "Galil.h"
#pragma once
#include "EmbeddedFunctions.h"
#include <iostream>
#include <stdint.h>

#include <string>

	/* Default constructor. Initialize variables, open Galil connection and allocate memory.
	* Should assign a default embedded functions that works with physical hardware and a
	* default Galil address as described in the assignment spec.
	*/
	Galil::Galil() : Functions(nullptr), g(0), ControlParameters{ 0.0, 0.0, 0.0 }, setPoint(0), savedAddress("192.168.0.120") {
		Functions = new EmbeddedFunctions();

		const GCStringIn addr = "192.168.0.120 -d";
		const GReturn rc = Functions->GOpen(addr, &g);
		if (rc != G_NO_ERROR) {
			/*delete Functions;
			Functions = nullptr;*/
			throw std::runtime_error(std::string("GOpen failed for default address '") +
				"192.168.0.120 -d" + "' - Code: " + std::to_string(rc));
		}
    };	
	/* Constructor with EmbeddedFunciton pre-initialised and passed in
	*/
	Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address)  : Functions(Funcs), g(0), ControlParameters{ 0.0, 0.0, 0.0 }, setPoint(0), savedAddress(address) {
		const GReturn rc = Functions->GOpen(address, &g);
		if (rc != G_NO_ERROR) {
			delete Functions;
			Functions = nullptr;
			throw std::runtime_error(std::string("GOpen failed for address '") +
				address + "' - Code: " + std::to_string(rc));
		}
	};	
	/* Copy constructor to copy the state of all elements within the object other.
	It should construct a new EmbeddedFunctions object and open a separate connection
	(i.e., each class will have a unique value of the GCon g). All other data members
	should be transferred.
	*/
	Galil::Galil(const Galil& other) : g(0), setPoint(other.setPoint) {
		Functions = new EmbeddedFunctions(*(other.Functions));

		for (int i = 0; i < 3; ++i) {
			ControlParameters[i] = other.ControlParameters[i];
		}

		savedAddress = other.savedAddress;

		const GReturn rc = Functions->GOpen(savedAddress.c_str(), &g);
		if (rc != G_NO_ERROR) {
			delete Functions;
			Functions = nullptr;
			throw std::runtime_error(std::string("Copy Constructor failed") +
				" - Code: " + std::to_string(rc));
		}
	};													
	
	// Move constructor deleted and should not be implemented.

	// Default destructor. Deallocate memory and close Galil connection.
	Galil::~Galil() {
		if (Functions != nullptr && g != 0) {
			Functions->GClose(g);
		}
		/*if (Functions != nullptr) {
			delete Functions;
			Functions = nullptr;
		}*/
	};												

	// DIGITAL OUTPUTS
	// Write to all 16 bits of digital output, 1 command to the Galil
	void Galil::DigitalOutput(uint16_t value) {
		std::string command = "OP " + std::to_string(value);

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("DigitalOutput failed: " + std::to_string(rc));
		}
	};

	//// Write to one byte, either high or low byte, as specified by user in 'bank'
	//// 0 = low, 1 = high
	void Galil::DigitalByteOutput(bool bank, uint8_t value) {
		int offset = (bank == 0) ? 0 : 8;

		for (int i = 0; i < 8; ++i) {
			int bit_index = offset + i;

			bool is_high = (value & (1 << i)) != 0;

			std::string command = (is_high ? "SB " : "CB ") + std::to_string(bit_index);

			GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

			if (rc != G_NO_ERROR) {
				throw std::runtime_error("DigitalByteOutput failed: " + command + " with code: " + std::to_string(rc));
			}
		}
	}
	//// Write single bit to digital outputs. 'bit' specifies which bit
	void Galil::DigitalBitOutput(bool val, uint8_t bit) {
		std::string command = (val ? "SB " : "CB ") + std::to_string(bit);

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("DigitalBitOutput failed: " + std::to_string(bit) + " with code: " + std::to_string(rc));
		}
	};


	//// DIGITAL INPUTS
	// Return the 16 bits of input data
	// Query the digital inputs of the GALIL, See Galil command library @IN
	uint16_t Galil::DigitalInput() {
		uint16_t result = 0;
		for (int i = 0; i <= 15; ++i) {
			std::string command = "MG @IN[" + std::to_string(i) + "]";

			GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

			if (rc != G_NO_ERROR) {
				throw std::runtime_error("Failed to read @IN[" + std::to_string(i) + "] with code: " + std::to_string(rc));
			}

			try {
				uint16_t bit_val = static_cast<uint16_t>(std::stod(prevResponse));
				if (bit_val == 1) {
					result |= (1 << (i));
				}
			}
			catch (const std::exception&) {
				throw std::runtime_error(std::string("Failed to parse @IN response: ") + prevResponse);
			}
		}

		return result;
	}
	// Read either high or low byte, as specified by user in 'bank'
	// 0 = low, 1 = high
	// A bank is one byte (8 bits). The low bank (0) is the first 8
	// bits (DI0-DI7) and the high bank (1) is the upper 8 bits
	// (DI8-DI15).
	uint8_t Galil::DigitalByteInput(bool bank) {
		uint8_t result = 0;
		int offset = bank ? 8 : 0;

		for (int i = 0; i < 8; ++i) {
			if (DigitalBitInput(offset + i)) {
				result |= (1 << i);
			}
		}

		return result;
	}
	// Read single bit from current digital inputs. Above functions
	// may use this function
	bool Galil::DigitalBitInput(uint8_t bit) {
		std::string command = "MG @IN[" + std::to_string(bit) + "]";

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("Failed to read @IN[" + std::to_string(bit) + "] with code: " + std::to_string(rc));
		}

		try {
			return static_cast<uint8_t>(std::stod(prevResponse)) == 1;
		}
		catch (const std::exception&) {
			throw std::runtime_error(std::string("Failed to parse @IN response: ") + prevResponse);
		}
	}					

	// Check the string response from the Galil to check that the last
	// command executed correctly. 1 = succesful.
	// A successful write indicates that a write command (sending a 
	// message to the Galil that does not have a response -- e.g., 
	// digitalOutput, analogOutput) has completed without errors.
	// This should validate some part of the Galil's response (it is 
	// up to you how this is completed) but should validly
	// differentiate when a write command has been completed 
	// successfully.
	// This will be called from your main function (do not call it
	// within your implementation functions of this Galil class.
	bool Galil::CheckSuccessfulWrite() {
		std::string response_str(prevResponse);

		if (response_str.find('?') != std::string::npos) {
			return false; 
		}

		return true;
	};

	// ANALOG FUNCITONS
	// Read Analog channel and return voltage			
	float Galil::AnalogInput(uint8_t channel) {
		std::string command = "MG @AN[" + std::to_string(channel) + "]";

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("Failed to read @AN[" + std::to_string(channel) + "] with code: " + std::to_string(rc));
		}

		try {
			return std::stof(prevResponse);
		}
		catch (const std::exception&) {
			throw std::runtime_error(std::string("Failed to parse @AN response: ") + prevResponse);
		}
	};
	
	// Write to any channel of the Galil, send voltages as
	// 2 decimal place in the command string
	void Galil::AnalogOutput(uint8_t channel, double voltage) {
		std::string command = "AO " + std::to_string(channel) + "," + std::to_string(voltage);

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("AnalogOutput failed: " + std::to_string(rc));
		}
	};

	// Configure the range of the input channel with
	// the desired range code
	void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
		std::string command = "AQ " + std::to_string(channel) + "," + std::to_string(range);

		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("AnalogInputRange failed: " + std::to_string(rc));
		}
	};
														

	// ENCODER
	// Manually Set the motor encoder value to zero (encoder channel 0)
	void Galil::WriteEncoder() {
		std::string command = "WE 0";
		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("WriteEncoder failed: " + std::to_string(rc));
		}
	};
								
	// Read from motor Encoder (encoder channel 0)
	int Galil::ReadEncoder() {
		std::string command = "QE 0";
		GReturn rc = Functions->GCommand(g, command.c_str(), prevResponse, sizeof(prevResponse), nullptr);

		if (rc != G_NO_ERROR) {
			throw std::runtime_error("ReadEncoder failed: " + std::to_string(rc));
		}
		return std::stoi(prevResponse);
	};

	// CONTROL FUNCTIONS
	// Set the desired setpoint for control loops, counts or counts/sec
	// // This should set it within the class not on the actual Galil.
	void Galil::setSetPoint(int s) {
		setPoint = s;
		std::cout << setPoint << std::endl;
	};
	
	// Gets the current setpoint stored in the class
	double Galil::getSetPoint() {
		std::cout << setPoint << std::endl;
		return setPoint;
	};

	// Set the proportional gain of the controller used in controlLoop() of Position/SpeedControl
	// This should set it within the class not on the actual Galil.
	void Galil::setKp(double gain) {
		ControlParameters[0] = gain;
	};
													
	// Gets the current proportional gain stored in the class
	double Galil::getKp() {
		return ControlParameters[0];
	};
										
	// Set the integral gain of the controller used in controlLoop()  of Position/SpeedControl
	// This should set it within the class not on the actual Galil.
	void Galil::setKi(double gain) {
		ControlParameters[1] = gain;
	};
													
	// Gets the current integral gain stored in the class
	double Galil::getKi() {
		return ControlParameters[1];
	};

	// Set the derivative gain of the controller used in controlLoop()  of Position/SpeedControl
	// This should set it within the class not on the actual Galil.
	void Galil::setKd(double gain) {
		ControlParameters[2] = gain;
	};
	// Gets the current derivative gain stored in the class
	double Galil::getKd() {
		return ControlParameters[2];
	};
	// Run the control loop. ReadEncoder() is the input to the loop. The motor is the output.	
	// The loop will run using the PID values specified in the data of this object, and has an
	// automatic timeout of 10s. You do NOT need to implement this function, it is defined in
	// GalilControl.lib
	//void Galil::PositionControl(bool debug, int Motorchannel) {};
	
	// same as above. Setpoint interpreted as counts per second
	//void Galil::SpeedControl(bool debug, int Motorchannel) {};


	//// OPERATOR OVERLOADS
	// Operator overload for '<<' operator. So the user can say cout << Galil;
	// This function should print out the output of GInfo and GVersion, with
	// two newLines after each.
	std::ostream& operator<<(std::ostream& output, Galil& galil) {
		char info[256] = "";
		char version[256] = "";
		galil.Functions->GInfo(galil.g, info, sizeof(info));
		galil.Functions->GVersion(version, sizeof(version));
		output << info << "\n\n";
		output << version << "\n\n";

		return output;
	};	
	
	// Copy assignment operator. This acts in the same way as the copy constructor
	// // (refer above for details).
	Galil& Galil::operator=(const Galil& other) {
		if (this == &other) {
			return *this;
		}

		this->setPoint = other.setPoint;
		for (int i = 0; i < 3; ++i) {
			this->ControlParameters[i] = other.ControlParameters[i];
		}

		if (this->g) {
			this->Functions->GClose(this->g);
		}
		if (this->Functions) {
			delete this->Functions;
		}

		this->Functions = new EmbeddedFunctions(*(other.Functions));

		this->savedAddress = other.savedAddress;

		const GReturn rc = this->Functions->GOpen(this->savedAddress.c_str(), &this->g);
		if (rc != G_NO_ERROR) {
			delete this->Functions;
			this->Functions = nullptr;
			throw std::runtime_error(std::string("Copy Assignment failed") +
				" - Code: " + std::to_string(rc));
		}

		return *this;
	};
	//																		
	
