#ifndef __COMMHANDLER_H__
#define __COMMHANDLER_H__

#include <queue>
#include "erl_interface.h"
#include "ei.h"

#include "CommandHandler.h"

/**
 */
class CommHandler
{
public:

	CommHandler(CommandHandler* commandHandler);
	~CommHandler();

	bool Open(int port);
	void Close();

	void Listen();
	
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
