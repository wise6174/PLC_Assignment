#include "EmbeddedFunctions.h"

// Constructor
EmbeddedFunctions::EmbeddedFunctions() {
	stream = nullptr;
	client = nullptr;
}


// Destructor
EmbeddedFunctions::~EmbeddedFunctions() {
	GClose();
}

/**
* Open a connection to a Galil Controller.
*
* @param address Null-terminated address string. Use direct connection (-d) to connect to hardware or simulator (e.g., "192.168.0.120 -d").
* @param port Integer value for the port to connect to.
*
* @throws error if one occurs.
*/
// TODO: complete this function.
void EmbeddedFunctions::GOpen(String^ address, const int port) {
	try {
		client = gcnew TcpClient(address, port);
		stream = client->GetStream();
		client->ReceiveTimeout = 5000;
		client->SendTimeout = 5000;
	}
	catch (System::Exception^ exception) {
		throw gcnew System::Exception("Failed to open TCP connection: " + exception->Message);
	};
}

/**
* Closes a connection to a Galil Controller.
 `GClose()` should be called whenever a program is finished with a controller. This includes when a program closes. A rule of thumb is that for every `GOpen()` call on a given connection, a `GClose()` call should be found on every code path. Failing to call GClose() may cause controller resources to not be released or can hang the process if there are outstanding asynchronous operations. The latter can occur, for example, if a call to GRead() times out and the process exits without calling GClose(). In this case, GRead() still has an outstanding asynchronous read pending. GClose() will terminate this operation allowing the process to exit correctly.
*
*
* @throws error if one occurs.
*/
// TODO: complete this function.
void EmbeddedFunctions::GClose() {
	if (stream != nullptr) {
		stream->Close();
		stream = nullptr;
	}
	if (client != nullptr) {
		client->Close();
		client = nullptr;
	}
}

/**
* Performs a *command-and-response* transaction on the connection.
* IMPORTANT: Commands being sent to the galil should be in the form of strings based on the command reference (uploaded to Moodle). You should choose commands appropriate for a given task and avoid those that rely on logical expressions. Commands chosen should send data directly to the output or read directly from the inputs.
* IMPORTANT: If the command string supplied is not terminated by a semicolon, you should append one.
*
* @param command Null-terminated command string to send to the controller.
*
* @return The reponse from the Galil.
* @throws error if one occurs.
*/
// TODO: complete this function.
String^ EmbeddedFunctions::GCommand(String^ command) { 

	return ""; 
}
