#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

#include "CommHandler.h"

void *spawnCommHandler(void * arg) {

	std::cout << "Creating CommHandler" << std::endl;

	CommandHandler *commandHandler = (CommandHandler*)arg;

	CommHandler* comm = new CommHandler(commandHandler);

	comm->Open(3456);

	comm->Listen();

	delete comm;

	return NULL;
}

int main(int argc, char **argv) {

	CommandHandler* commandHandler = new CommandHandler();

	pthread_t t;
	
	pthread_create(&t, NULL, spawnCommHandler, (void *)commandHandler);

	//pthread_join(t, NULL);

	std::cout << "Await commands \n" << std::endl;

	if (commandHandler == NULL) {
		std::cout << "Didnt work \n" << std::endl;
	} else {
		std::cout << "Handler" << commandHandler << std::endl;

	}

	while (true) {

		std::this_thread::sleep_for (std::chrono::seconds(5));

		commandHandler->Ping();

		std::cout << "Check commands" << std::endl;

		if (Command* cmd = commandHandler->GetNextCommand()) {

			if (typeid(RecordCommand) == typeid(cmd)) {
				std::cout << "Got a Record \n" << std::endl;
			} else if (typeid(PredictCommand) == typeid(cmd)) {
				std::cout << "Got a Predict\n" << std::endl;
			}
			else {
				std::cout << "Got another command: " << typeid(cmd).name() << std::endl;	
			}
			break;
		}

	}

	std::cout << "Program done" << std::endl;

	return 0;
}