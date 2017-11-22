#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <queue>
#include "erl_interface.h"
#include "ei.h"

#include "CommandHandler.h"
#include "EventHandler.h"

/**
 */
class Communcation
{
public:

	Communcation(CommandHandler* commandHandler, EventHandler* eventHandler);
	~Communcation();

	bool Init(int port);
	
private:

	int SetupSocket(int port);

	int mPort;
	int mListen;                              /* Listen socket */
  	int mFD;
  	bool mLoop;
  	ErlConnect mConn;

  	CommandHandler* mCommandHandler;
  	EventHandler* mEventHandler;
};

#endif
