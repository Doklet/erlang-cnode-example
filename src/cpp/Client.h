#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "EventHandler.h"

/**
 */
class Client
{
public:

	Client(const char* deviceId, EventHandler* eventHandler);

	void Connect();

	void StartListenForEvents();
	
private:

	std::string mDeviceId;
  	EventHandler* mEventHandler;

  	int mFD;
};

#endif
