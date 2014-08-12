/*!
 * \file
 * \brief
 */

#ifndef CVADAPTIVETHRESHOLD_PROCESSOR_HPP_
#define CVADAPTIVETHRESHOLD_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <opencv2/core/core.hpp>
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
	
	void prepareInterface();
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

	Base::DataStreamIn <Mat> in_img;
	Base::DataStreamOut <Mat> out_img;

	cv::Mat thresholdedImage;

	void onNewImage();
};

}//: namespace CvAdaptiveThreshold
}//: namespace Processors


/*
 * Register processor component.
 */REGISTER_COMPONENT("CvAdaptiveThreshold", Processors::CvAdaptiveThreshold::CvAdaptiveThreshold_Processor)

#endif /* CVADAPTIVETHRESHOLD_PROCESSOR_HPP_ */
