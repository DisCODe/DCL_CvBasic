/*!
 * \file
 * \brief
 * \author Jan Figat
 */

#include <memory>
#include <string>

#include "ViewSequence.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace ViewSequence {

ViewSequence::ViewSequence(const std::string & name) :
		Base::Component(name)  {

}

ViewSequence::~ViewSequence() {
}

void ViewSequence::prepareInterface() {
	// Register data streams, events and event handlers HERE!

}

bool ViewSequence::onInit() {

	return true;
}

bool ViewSequence::onFinish() {
	return true;
}

bool ViewSequence::onStop() {
	return true;
}

bool ViewSequence::onStart() {
	return true;
}



} //: namespace ViewSequence
} //: namespace Processors
