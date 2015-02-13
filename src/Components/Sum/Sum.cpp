/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "Sum.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Sum {

Sum::Sum(const std::string & name) :
		Base::Component(name),
		norm("norm", 1)
{
	LOG(LTRACE) << "Hello Sum\n";

	registerProperty(norm);
}

Sum::~Sum() {
}

void Sum::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&Sum::onNewImage, this));
	addDependency("onNewImage", &in_img1);
	addDependency("onNewImage", &in_img2);

	// Input and output data streams.
	registerStream("in_img1", &in_img1);
	registerStream("in_img2", &in_img2);
	registerStream("out_img", &out_img);
}

bool Sum::onInit() {

	return true;
}

bool Sum::onFinish() {
	return true;
}

bool Sum::onStop() {
	return true;
}

bool Sum::onStart() {
	return true;
}

void Sum::onNewImage()
{
	LOG(LTRACE) << "Sum::onNewImage\n";
	try {
		cv::Mat img1 = in_img1.read();
		cv::Mat img2 = in_img2.read().clone();

		// TODO: add comparison of size1 & size2.
		//cv::Size size = img1.size();

		// Create a matrix with the adequate size and depth.
		tmp.create(img1.size(), img1.depth());

		// Sum the images (normalized).
		tmp = (img1 + img2) * norm;

		// Write the result to the output.
		out_img.write(tmp);
	} catch (...) {
		LOG(LERROR) << "Sum::onNewImage failed\n";
	}
}



} //: namespace Sum
} //: namespace Processors
