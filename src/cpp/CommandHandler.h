#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__

#include <queue>
#include <string>

enum CommandType {
	RECORD,
	PREDICT
};

struct RecordCommand
{
	RecordCommand(const char* path) {
		this->recordPath = std::string(path);
	}

	std::string recordPath;
};

struct PredictCommand
{
	PredictCommand(const char* path) {
		this->filePath = std::string(path);
	}

	std::string filePath;
};

struct Command
{
	Command(RecordCommand* cmd) {
		this->type = RECORD;
		this->recordCommand = cmd;
	};

	Command(PredictCommand* cmd) {
		this->type = PREDICT;
		this->predictCommand = cmd;
	};

	~Command() {
		delete recordCommand;
		recordCommand = NULL;
		delete predictCommand;
		predictCommand = NULL;
	};

	CommandType type;
	RecordCommand* recordCommand;
	PredictCommand* predictCommand;
};

struct CommandResult
{
	CommandResult(Command* cmd, const char* result) {
		this->command = cmd;
		this->result = std::string(result);
	};

	~CommandResult() {
		delete command;
	};

	Command* command;
	std::string result;
};

/**
 */
class CommandHandler
{
public:

	void Ping();

	Command* CreateRecordCommand(const char *filePath);
	Command* CreatePredictCommand(const char *filePath);

	CommandResult* CreateResult(Command* command, const char* result);
	void DeleteResult(CommandResult* result);

	Command* GetNextCommand();
	CommandResult* GetNextResult();

private:

  	std::queue<Command*> mCommands;
  	std::queue<CommandResult*> mResults;
};

#endif
