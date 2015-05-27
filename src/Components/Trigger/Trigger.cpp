/*!
 * \file
 * \brief
 * \author Micha Laszkowski
 */

#include <memory>
#include <string>

#include "Trigger.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Trigger {

Trigger::Trigger(const std::string & name) :
	Base::Component(name),
	prop_auto_trigger("auto_trigger", false)
{
	registerProperty(prop_auto_trigger);
}

Trigger::~Trigger() {
}

void Trigger::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("out_trigger", &out_trigger);
	registerStream("in_stop_auto_trigger", &in_stop_auto_trigger);

	// Register handlers
	registerHandler("Sent Trigger", boost::bind(&Trigger::onTriggerButtonPressed, this));

	registerHandler("onAutoTrigger", boost::bind(&Trigger::onAutoTrigger, this));
	addDependency("onAutoTrigger", NULL);

	registerHandler("onStopAutoTrigger", boost::bind(&Trigger::onStopAutoTrigger, this));
	addDependency("onStopAutoTrigger", &in_stop_auto_trigger);
}

bool Trigger::onInit() {
	// Reset flag.
	triggered_flag = false;
	return true;
}

bool Trigger::onFinish() {
	return true;
}

bool Trigger::onStop() {
	return true;
}

bool Trigger::onStart() {
	return true;
}


void Trigger::onTriggerButtonPressed() {
	CLOG(LDEBUG) << "Trigger::onTriggerButtonPressed";
	triggered_flag = true;
}


void Trigger::onAutoTrigger() {
	CLOG(LDEBUG) << "Trigger::onAutoTrigger";
	if (prop_auto_trigger || triggered_flag){
		triggered_flag = false;
		Base::UnitType t;
		out_trigger.write(t);
	}//: if
}


void Trigger::onStopAutoTrigger() {
	CLOG(LDEBUG) << "Trigger::onStopAutoTrigger";
	in_stop_auto_trigger.read();
	prop_auto_trigger = false;
	// TODO: DisCODe GUI fix is required to notice this. As a result turning autotrigger once again requires checking off and on.
}


} //: namespace Trigger
} //: namespace Processors
