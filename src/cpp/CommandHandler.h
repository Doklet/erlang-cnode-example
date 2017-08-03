#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__

#include <queue>
#include <string>

enum CommandType {
	RECORD = 0,
	PREDICT = 1
};

struct RecordCommand
{
	std::string *recordPath;
};

struct PredictCommand
{
	std::string *filePath;
};

struct Command
{
	Command(RecordCommand cmd) {
		this.type = CommandType.RECORD;
		this->recordCommand = cmd;
	};

	Command(PredictCommand cmd) {
		this.type = CommandType.PREDICT;
		this->predictCommand = cmd;
	};

	CommandType type;
	RecordCommand* recordCommand;
	PredictCommand* predictCommand;
};

/**
 */
class CommandHandler
{
public:

	void Ping();

	void AddCommand(Command* command);

	Command* GetNextCommand();

private:

  	std::queue<Command*> mCommands;
};

#endif
