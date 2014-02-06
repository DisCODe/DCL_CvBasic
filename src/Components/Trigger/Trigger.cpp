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
		Base::Component(name)  {

}

Trigger::~Trigger() {
}

void Trigger::prepareInterface() {
	// Register data streams, events and event handlers HERE!
registerStream("out_trigger", &out_trigger);
	// Register handlers
	h_trigger.setup(boost::bind(&Trigger::trigger, this));
	registerHandler("trigger", &h_trigger);
	addDependency("trigger", NULL);

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

void Trigger::trigger() {
	out_trigger.write(true);
}



} //: namespace Trigger
} //: namespace Processors
