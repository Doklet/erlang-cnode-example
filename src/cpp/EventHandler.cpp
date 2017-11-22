#include "EventHandler.h"

#include <assert.h>

Event* EventHandler::CreateSystemReadyEvent()
{
	Event* event = new Event();
	event->type = EventType::SYSTEM_READY;
	event->ready = new SystemReady();

	this->mEvents.push(event);

	return event;
}

Event* EventHandler::CreateItemClassifiedEvent(const char *name, std::list<Label> labels)
{
	assert(name != NULL);

	Classification* classification = new Classification();
	classification->labels = labels;

	Event* event = new Event();
	event->type = EventType::CLASSIFICATION;
	event->classification = classification;

	this->mEvents.push(event);

	return event;
}

void EventHandler::DeleteEvent(Event* event)
{
	if (event != NULL)
	{
		if (event->classification != NULL)
		{
			delete event->classification;
			event->classification = NULL;
		}

		if (event->ready != NULL)
		{
			delete event->ready;
			event->ready = NULL;
		}

		delete event;
		event = NULL;
	}
}

Event* EventHandler::GetNextEvent()
{

	// TODO need to make this one thread safe

  if (!this->mEvents.empty()) {

    Event* front = this->mEvents.front();

    this->mEvents.pop();

    return front;
  }

  return NULL;
}
