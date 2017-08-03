#include "CommandHandler.h"

#include <iostream>

void CommandHandler::Ping() {

	std::cout<< "CommandHandler Pong \n";	
}

void CommandHandler::AddCommand(Command* command) {

	// TODO need to make this one thread safe

	return this->mCommands.push(command);
}

Command* CommandHandler::GetNextCommand() {

	// TODO need to make this one thread safe

  if (!this->mCommands.empty()) {

    Command* front = this->mCommands.front();

    this->mCommands.pop();

    return front;
  }

  return NULL;
}