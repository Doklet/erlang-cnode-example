#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <string>
#include <queue>
#include <list>

enum EventType {
	SYSTEM_READY,
	PRODUCT_ENTER,
	PRODUCT_LEAVE,
	CLASSIFICATION
};

struct SystemReady {};
struct ProductEnter {};
struct ProductLeave {};

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

struct Event
{
	EventType 				type;

	struct SystemReady* 	ready;
	struct ProductEnter* 	enter;
	struct ProductLeave* 	leave;
	struct Classification* 	classification;	
};


class EventHandler
{
public:

	Event* 	CreateSystemReadyEvent();
	Event* 	CreateProductEnterEvent();
	Event* 	CreateProductLeaveEvent();
	Event* 	CreateItemClassifiedEvent(const char* name, std::list<Label> labels);

	void 	DeleteEvent(Event* event);

	Event*	GetNextEvent();
	
private:

	std::queue<Event*> mEvents;
};

#endif
