#ifndef ROCKETINVADERSEVENT_H
#define ROCKETINVADERSEVENT_H

#include <Rocket/Core/EventListener.h>

/**
	@author Peter Curry
 */

class GRocketEvent : public Rocket::Core::EventListener
{
public:
	GRocketEvent(const Rocket::Core::String& value);
	virtual ~GRocketEvent();

	/// Sends the event value through to Invader's event processing system.
	virtual void ProcessEvent(Rocket::Core::Event& event);

	/// Destroys the event.
	virtual void OnDetach(Rocket::Core::Element* element);

private:
	Rocket::Core::String value;
};

#endif
