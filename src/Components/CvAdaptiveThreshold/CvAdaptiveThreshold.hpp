/*!
 * \file
 * \brief
 */

#ifndef CVADAPTIVETHRESHOLD_PROCESSOR_HPP_
#define CVADAPTIVETHRESHOLD_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <cv.h>
using namespace cv;

namespace Processors {
namespace CvAdaptiveThreshold {

/*!
 * \class CvAdaptiveThreshold_Processor
 * \brief CvAdaptiveThreshold processor class.
 */
class CvAdaptiveThreshold_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvAdaptiveThreshold_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvAdaptiveThreshold_Processor();
protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

private:

	Base::Property <double> maxValue;
	Base::Property <std::string> method;
	Base::Property <std::string> thresholdType;
	Base::Property <int> blockSize;
	Base::Property <double> C;

	Base::EventHandler <CvAdaptiveThreshold_Processor> h_onNewImage;
	Base::DataStreamIn <Mat> in_img;
	Base::Event *newImage;
	Base::DataStreamOut <Mat> out_img;

	cv::Mat thresholdedImage;

	void onNewImage();
};

}//: namespace CvAdaptiveThreshold
}//: namespace Processors


/*
 * Register processor component.
 */REGISTER_PROCESSOR_COMPONENT("CvAdaptiveThreshold", Processors::CvAdaptiveThreshold::CvAdaptiveThreshold_Processor, Common::Panel_Empty)

#endif /* CVADAPTIVETHRESHOLD_PROCESSOR_HPP_ */
