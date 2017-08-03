#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

#include "CommHandler.h"

void *spawnCommHandler(void * arg) {

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

	std::cout << "Await commands \n" << std::endl;

	while (true) {

		std::this_thread::sleep_for (std::chrono::seconds(5));

		if (Command* cmd = commandHandler->GetNextCommand()) {
			switch (cmd->type) {
				case RECORD:
					std::cout << "Got a record command\n";
					std::cout << "recordPath: " << cmd->recordCommand->recordPath << std::endl;
					break;
				case PREDICT:
					std::cout << "Got a predict command:\n" << std::endl;
					std::cout << "filePath: " << cmd->predictCommand->filePath << std::endl;
					break;
				default:
					std::cout << "Got a unkown command" << std::endl;
			}

			delete cmd;
		}
	}

	std::cout << "Program done" << std::endl;

	return 0;
}