#include "CommandHandler.h"

#include <iostream>

void CommandHandler::Ping() {

	std::cout<< "CommandHandler Pong \n";	
}

void CommandHandler::AddCommand(Command* command) {

	// TODO need to make this one thread safe

	std::cout<< "CommandHandler add command \n";

	return this->mCommands.push(command);
}

Command* CommandHandler::GetNextCommand() {

// TODO need to make this one thread safe

  std::cout<< "CommandHandler getNext command \n";

  if (!this->mCommands.empty()) {

    return this->mCommands.front();
  }

  return NULL;
}