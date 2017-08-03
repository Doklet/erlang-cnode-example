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
		if (recordCommand != NULL)
			delete recordCommand;
		
		if (predictCommand != NULL)
			delete predictCommand;
	}

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
