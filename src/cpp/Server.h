#ifndef __SERVER_H__
#define __SERVER_H__

#include <queue>
#include "erl_interface.h"
#include "ei.h"

#include "CommandHandler.h"

/**
 */
class Server
{
public:

	Server(CommandHandler* commandHandler);
	~Server();

	/*
		Used by the command communication, FIXME seperate class better!
	*/
	bool Open(int port);
	void Close();
	void Listen();

	// void StartListenForEvents();
	
private:

	int SetupSocket(int port);

	int mPort;
	int mListen;                              /* Listen socket */
  	int mFD;
  	bool mLoop;
  	ErlConnect mConn;

  	CommandHandler* mCommandHandler;
};

#endif
