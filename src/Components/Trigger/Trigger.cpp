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
	// Register handlers
	registerHandler("Sent Trigger", boost::bind(&Trigger::onTriggerButtonPressed, this));

	registerHandler("onAutoTrigger", boost::bind(&Trigger::onAutoTrigger, this));
	addDependency("onAutoTrigger", NULL);
}

bool Trigger::onInit() {

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
	Base::UnitType t;
	out_trigger.write(t);
}


void Trigger::onAutoTrigger() {
	CLOG(LDEBUG) << "Trigger::onAutoTrigger";
	if (prop_auto_trigger){
		Base::UnitType t;
		out_trigger.write(t);
	}//: if
}



} //: namespace Trigger
} //: namespace Processors
