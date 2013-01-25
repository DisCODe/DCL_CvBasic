/*!
 * \file
 * \brief
 */

#ifndef CVHOUGHCIRCLES_PROCESSOR_HPP_
#define CVHOUGHCIRCLES_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Circles/Circles.hpp"

#include <cv.h>
using namespace cv;

namespace Processors {
namespace CvHoughCircles {

/*!
 * \class CvHoughCircles_Processor
 * \brief CvHoughCircles processor class.
 */
class CvHoughCircles_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvHoughCircles_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvHoughCircles_Processor();

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
	Base::Property<double> inverseRatioOfAccumulatorResolution;
	Base::Property<double> minDist;
	Base::Property<double> cannyHigherThreshold;
	Base::Property<double> accumulatorThreshold;
	Base::Property<int> minCircleRadius;
	Base::Property<int> maxCircleRadius;

	/** New image event handler. */
	Base::EventHandler <CvHoughCircles_Processor> h_onNewImage;
	/** Image stream. */
	Base::DataStreamIn <cv::Mat> in_img;

	Base::DataStreamOut <Types::Circles> out_circles;
	Base::Event* newCircles;

	void onNewImage();
};

}//: namespace CvHoughCircles
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvHoughCircles", Processors::CvHoughCircles::CvHoughCircles_Processor, Common::Panel_Empty)

#endif /* CVHOUGHCIRCLES_PROCESSOR_HPP_ */
