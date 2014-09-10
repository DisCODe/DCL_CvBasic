/*!
 * \file
 * \brief
 * \author Tomek Kornuta
 */

#include <memory>
#include <string>

#include "RGBLUT.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace RGBLUT {

RGBLUT::RGBLUT(const std::string & name) :
		Base::Component(name),
		m_red_threshold_low("red.threshold.low", 0, "range"),
		m_red_threshold_high("red.threshold.high", 255, "range"),
		m_green_threshold_low("green.threshold.low", 0, "range"),
		m_green_threshold_high("green.threshold.high", 255, "range"),
		m_blue_threshold_low("blue.threshold.low", 0, "range"),
		m_blue_threshold_high("blue.threshold.high", 255, "range")
{
	// Constraints.
	m_red_threshold_low.addConstraint("0");
	m_red_threshold_low.addConstraint("255");
	m_red_threshold_high.addConstraint("0");
	m_red_threshold_high.addConstraint("255");

	m_green_threshold_low.addConstraint("0");
	m_green_threshold_low.addConstraint("255");
	m_green_threshold_high.addConstraint("0");
	m_green_threshold_high.addConstraint("255");

	m_blue_threshold_low.addConstraint("0");
	m_blue_threshold_low.addConstraint("255");
	m_blue_threshold_high.addConstraint("0");
	m_blue_threshold_high.addConstraint("255");

	registerProperty(m_red_threshold_low);
	registerProperty(m_red_threshold_high);
	registerProperty(m_green_threshold_low);
	registerProperty(m_green_threshold_high);
	registerProperty(m_blue_threshold_low);
	registerProperty(m_blue_threshold_high);

	LOG(LTRACE) << "Hello RGBLUT\n";
}

RGBLUT::~RGBLUT() {
}

void RGBLUT::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerHandler("onNewImage", boost::bind(&RGBLUT::onNewImage, this));

	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);

	// Add default dependency of the onNewImage.
	addDependency("onNewImage", &in_img);
}

bool RGBLUT::onInit() {

	return true;
}

bool RGBLUT::onFinish() {
	return true;
}

bool RGBLUT::onStop() {
	return true;
}

bool RGBLUT::onStart() {
	return true;
}

void RGBLUT::onNewImage()
{
	LOG(LTRACE) << "RGBLUT::onNewImage\n";
	try {
		cv::Mat rgb_img = in_img.read();

		cv::Size size = rgb_img.size();

		tmp_img.create(size, CV_8UC1);

		// Check the arrays for continuity and, if this is the case,
		// treat the arrays as 1D vectors
		if (rgb_img.isContinuous() && tmp_img.isContinuous()) {
			size.width *= size.height;
			size.height = 1;
		}
		size.width *= 3;

		for (int i = 0; i < size.height; i++) {
			// when the arrays are continuous,
			// the outer loop is executed only once
			// if not - it's executed for each row
			const uchar* rgb_p = rgb_img.ptr <uchar> (i);
			uchar* tmp_p = tmp_img.ptr <uchar> (i);

			int j, k = 0;
			for (j = 0; j < size.width; j += 3) {
				uchar r = rgb_p[j];
				uchar g = rgb_p[j + 1];
				uchar b = rgb_p[j + 2];

				uchar val = 255;
				//  Red.
				if ((r < m_red_threshold_low) || (r > m_red_threshold_high))
					val = 0;
				//  Green.
				else if ((g < m_green_threshold_low) || (g > m_green_threshold_high))
					val = 0;
				//  blue.
				else if ((b < m_blue_threshold_low) || (b > m_blue_threshold_high))
					val = 0;

				tmp_p[k] = val;

				++k;
			}
		}

		// Write output to stream.
		out_img.write(tmp_img);
	}
	catch (Common::DisCODeException& ex) {
		LOG(LERROR) << ex.what() << "\n";
		ex.printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		LOG(LERROR) << ex;
	}
	catch (...) {
		LOG(LERROR) << "RGBLUT::onNewImage failed\n";
	}
}



} //: namespace RGBLUT
} //: namespace Processors
