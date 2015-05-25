/*!
 * \file
 * \brief
 * \author Maciej Stefanczyk
 */

#include <memory>
#include <string>

#include "MaskAggregator.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace MaskAggregator {

MaskAggregator::MaskAggregator(const std::string & name) :
		Base::Component(name)  {

}

MaskAggregator::~MaskAggregator() {
}

void MaskAggregator::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_mask", &in_mask);
	registerStream("out_mask", &out_mask);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&MaskAggregator::onNewImage, this));
	addDependency("onNewImage", &in_mask);

}

bool MaskAggregator::onInit() {

	return true;
}

bool MaskAggregator::onFinish() {
	return true;
}

bool MaskAggregator::onStop() {
	return true;
}

bool MaskAggregator::onStart() {
	return true;
}

void MaskAggregator::onNewImage() {
	cv::Mat mask = in_mask.read().clone();
	
	while(!in_mask.empty()) {
		in_mask.read().copyTo(mask, mask);
	}
	
	out_mask.write(mask);
}



} //: namespace MaskAggregator
} //: namespace Processors
