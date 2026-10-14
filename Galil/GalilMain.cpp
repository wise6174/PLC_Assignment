#include "Galil.h"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

#using <System.dll>	
using namespace System;
using namespace System::Threading;

/**
 * Hints for completing this task
 *  - Use std::this_thread::sleep_for(100ms) to sleep for 100 ms (C++ Syntax)
 *  - Use Threading::Thread::Sleep(100) to sleep for 100 ms (CLR Syntax)
 *	- Use std::stoi to convert string to an integer (C++ Syntax)
 *		Signature: int stoi(const std::string& str, size_t* pos = nullptr, int base = 10);
 *  - Use Convert::ToInt32 to convert string to an integer (CLR Syntax)
 *		Signature: int Convert::ToInt32(String^ str);
 *  - You might find std::min and std::max useful
 */


int main(void) {
	EmbeddedFunctions func;
	Galil g(&func, "192.168.0.120 -d");

	/* Your Code Here ... */
	g.DigitalOutput(0);
	//4,7,2
	float V_AIN = 0.0;
	int d = 0;
	int digitCounter = 0;
	int correctCounter = 0;
	while (!Console::KeyAvailable && digitCounter < 3) {
		V_AIN = g.AnalogInput(0);

		d = floor(Convert::ToInt32(V_AIN));
		if (d > 9) {
			d = 9;
		}
		g.DigitalByteOutput(0, d);
		if (d == 4 && digitCounter == 0) {
			correctCounter++;
		}
		else if (d == 7 && digitCounter == 1) {
			correctCounter++;
		}
		else if (d == 2 && digitCounter == 2) {
			correctCounter++;
		}
		digitCounter++;
		int digit = g.DigitalByteInput(0);
		g.DigitalByteOutput(1, digitCounter);
	}
	if (correctCounter == 3) {
		std::cout << "UNLOCKED" << std::endl;
		int i = 0;
		while (i<3) {
			g.DigitalOutput(15);
			g.DigitalOutput(0);
			Threading::Thread::Sleep(500);
			i++;
		}
	}
	else {
		std::cout<< "WRONG" << std::endl;
		g.DigitalOutput(0);
		g.AnalogOutput(0, 3);
		Threading::Thread::Sleep(500);
		g.AnalogOutput(0, 0);
	}
	

	/* End User Code */
	return 0;
}