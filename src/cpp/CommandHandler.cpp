#include "CommandHandler.h"

#include <iostream>

void CommandHandler::Ping() {

	std::cout<< "CommandHandler Pong \n";
}

Command* CommandHandler::CreateRecordCommand(const char *path){

	RecordCommand* recordCmd = new RecordCommand(path);

	Command* cmd = new Command(recordCmd);

	this->mCommands.push(cmd);

	return cmd;
}

Command* CommandHandler::CreatePredictCommand(const char *path){

	PredictCommand* predictCmd = new PredictCommand(path);

	Command* cmd = new Command(predictCmd);

	this->mCommands.push(cmd);

	return cmd;
}

CommandResult* CommandHandler::CreateResult(Command* command, const char* result) {

	// TODO need to make this one thread safe

	CommandResult* cmdResult = new CommandResult(command, result);

	this->mResults.push(cmdResult);

	return cmdResult;
}

void CommandHandler::DeleteResult(CommandResult* result) {
	delete result;
	result = NULL;
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

CommandResult* CommandHandler::GetNextResult() {

	// TODO need to make this one thread safe

  if (!this->mResults.empty()) {

    CommandResult* front = this->mResults.front();

    this->mResults.pop();

    return front;
  }

  return NULL;
}

