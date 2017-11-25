#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

#include "Server.h"
#include "Client.h"

void *spawnServer(void * arg) {

	CommandHandler *commandHandler = (CommandHandler*)arg;

	Server* server = new Server(commandHandler);

	server->Open(3457);

	server->Listen();

	delete server;

	return NULL;
}

void *spawnClient(void * arg) {

	EventHandler *eventHandler = (EventHandler*)arg;

	Client* client = new Client("Camera0", eventHandler);

	client->Connect();

	client->StartListenForEvents();

	delete client;

	return NULL;
}

int main(int argc, char **argv) {

	CommandHandler* commandHandler = new CommandHandler();
	EventHandler* eventHandler = new EventHandler();

	pthread_t server_t, client_t;
	
	// pthread_create(&server_t, NULL, spawnServer, (void *)commandHandler);
	pthread_create(&client_t, NULL, spawnClient, (void *)eventHandler);

	std::cout << "Going into sleep before first batch \n\r";

	std::this_thread::sleep_for (std::chrono::milliseconds(10000));

	eventHandler->CreateSystemReadyEvent();
	eventHandler->CreateProductEnterEvent();
	eventHandler->CreateProductLeaveEvent();

	std::cout << "Sending first batch \n\r";

	for (int i = 0; i < 10; ++i)
	{
		/* code */
		eventHandler->CreateItemClassifiedEvent("My event", std::list<Label>());
	}

	std::cout << "Sending second batch \n\r";

	std::this_thread::sleep_for (std::chrono::milliseconds(10000));

	for (int i = 0; i < 10; ++i)
	{
		/* code */
		eventHandler->CreateItemClassifiedEvent("My event", std::list<Label>());
	}

	std::cout << "Going into sleep \n\r";

	std::this_thread::sleep_for (std::chrono::milliseconds(1000));

	// while (true) {

	// 	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	// 	if (Command* cmd = commandHandler->GetNextCommand()) {
	// 		switch (cmd->type) {
	// 			case RECORD:
	// 				std::cout << "Got a record command\n\r";
	// 				std::cout << "recordPath: " << cmd->recordCommand->recordPath << std::endl;
	// 				commandHandler->CreateResult(cmd, "Record result");
	// 				break;
	// 			case PREDICT:
	// 				std::cout << "Got a predict command:\n\r";
	// 				std::cout << "filePath: " << cmd->predictCommand->filePath << std::endl;
	// 				//commandHandler->CreateResult(cmd, "Predict result");
	// 				eventHandler->CreateItemClassifiedEvent("My event", std::list<Label>());
	// 				break;
	// 			default:
	// 				std::cout << "Got a unkown command\n\r";
	// 		}

	// 		// TODO need to fix this, crash on TX1
	// 		// delete cmd;
	// 	}
	// }

	std::cout << "Program done \n\r";

	return 0;
}