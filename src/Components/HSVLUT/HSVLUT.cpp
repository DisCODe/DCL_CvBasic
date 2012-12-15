/*!
 * \file
 * \brief
 * \author Tomek Kornuta
 */

#include <memory>
#include <string>

#include "HSVLUT.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace HSVLUT {

HSVLUT::HSVLUT(const std::string & name) :
		Base::Component(name),
		m_hue_threshold_low("hue.threshold.low", 0, "range"),
		m_hue_threshold_high("hue.threshold.high", 180, "range"),
		m_sat_threshold_low("saturation.threshold.low", 0, "range"),
		m_sat_threshold_high("saturation.threshold.high", 255, "range"),
		m_val_threshold_low("value.threshold.low", 0, "range"),
		m_val_threshold_high("value.threshold.high", 255, "range")
{
	// Constraints.
	m_hue_threshold_low.addConstraint("0");
	m_hue_threshold_low.addConstraint("180");
	m_hue_threshold_high.addConstraint("0");
	m_hue_threshold_high.addConstraint("180");

	m_sat_threshold_low.addConstraint("0");
	m_sat_threshold_low.addConstraint("255");
	m_sat_threshold_high.addConstraint("0");
	m_sat_threshold_high.addConstraint("255");

	m_val_threshold_low.addConstraint("0");
	m_val_threshold_low.addConstraint("255");
	m_val_threshold_high.addConstraint("0");
	m_val_threshold_high.addConstraint("255");

	registerProperty(m_hue_threshold_low);
	registerProperty(m_hue_threshold_high);
	registerProperty(m_sat_threshold_low);
	registerProperty(m_sat_threshold_high);
	registerProperty(m_val_threshold_low);
	registerProperty(m_val_threshold_high);

	LOG(LTRACE) << "Hello HSVLUT\n";
}

HSVLUT::~HSVLUT() {
}

void HSVLUT::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &HSVLUT::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);

	// Add default dependency of the onNewImage.
	addDependency("onNewImage", &in_img);
}

bool HSVLUT::onInit() {

	return true;
}

bool HSVLUT::onFinish() {
	return true;
}

bool HSVLUT::onStop() {
	return true;
}

bool HSVLUT::onStart() {
	return true;
}

void HSVLUT::onNewImage()
{
	LOG(LTRACE) << "HSVLUT::onNewImage\n";
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
				uchar h = rgb_p[j];
				uchar s = rgb_p[j + 1];
				uchar v = rgb_p[j + 2];

				uchar val = 255;
				//  Red.
				if ((h < m_hue_threshold_low) || (h > m_hue_threshold_high))
					val = 0;
				//  Green.
				else if ((s < m_sat_threshold_low) || (s > m_sat_threshold_high))
					val = 0;
				//  val.
				else if ((v < m_val_threshold_low) || (v > m_val_threshold_high))
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
		LOG(LERROR) << "HSVLUT::onNewImage failed\n";
	}
}



} //: namespace HSVLUT
} //: namespace Processors
