#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <string>
#include <queue>
#include <list>

enum EventType {
	SYSTEM_READY,
	CLASSIFICATION
};

struct Label
{
	int 		index;
	std::string name;
	float 		score;
};

struct Classification
{
	std::list<Label> labels;
};

struct SystemReady
{
	
};

struct Event
{
	EventType 				type;

	struct SystemReady* 	ready;
	struct Classification* 	classification;	
};


class EventHandler
{
public:

	Event* 	CreateSystemReadyEvent();
	Event* 	CreateItemClassifiedEvent(const char* name, std::list<Label> labels);

	void 	DeleteEvent(Event* event);

	Event*	GetNextEvent();
	
private:

	std::queue<Event*> mEvents;
};

#endif
