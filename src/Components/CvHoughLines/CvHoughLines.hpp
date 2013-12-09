/*!
 * \file
 * \brief
 */

#ifndef CVHOUGHLINES_PROCESSOR_HPP_
#define CVHOUGHLINES_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Line.hpp"
#include "Types/DrawableContainer.hpp"

#include <opencv2/core/core.hpp>
using namespace cv;

namespace Processors {
namespace CvHoughLines {

/*!
 * \class CvHoughLines_Processor
 * \brief CvHoughLines processor class.
 */
class CvHoughLines_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvHoughLines_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvHoughLines_Processor();

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
	Base::Property<int> threshold;
	Base::Property<double> minLineLength;
	Base::Property<double> maxLineGap;

	/** New image event handler. */
	Base::EventHandler <CvHoughLines_Processor> h_onNewImage;
	/** Image stream. */
	Base::DataStreamIn <cv::Mat> in_img;

	Base::DataStreamOut <Types::DrawableContainer> out_lines;

	void onNewImage();
};

}//: namespace CvHoughLines
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvHoughLines", Processors::CvHoughLines::CvHoughLines_Processor)

#endif /* CVHOUGHLINES_PROCESSOR_HPP_ */
