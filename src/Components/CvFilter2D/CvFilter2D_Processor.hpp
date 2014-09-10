/*!
 * \file CvFilter2D_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVFILTER2D_PROCESSOR_HPP_
#define CVFILTER2D_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include "Types/MatrixTranslator.hpp"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


/**
 * \defgroup CvFilter2D CvFilter2D
 * \ingroup Processors
 *
 * Convolves an image with the kernel
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 *
 * \par Properties:
 *
 * \prop{size,cv::Size,3 3}
 * Kernel size
 * \prop{kernel,cv::Mat,&nbsp;}
 * Kernel itself
 * \prop{norm,double,1.0}
 * Normalisation factor
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/image_filtering.html#filter2D
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvFilter2D {

using namespace cv;


/*!
 * \class CvFilter2D_Processor
 * \brief Convolves an image with the kernel
 */
class CvFilter2D_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvFilter2D_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvFilter2D_Processor();

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


	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	Base::Property<cv::Mat, Types::MatrixTranslator> kernel;
	Base::Property<double> norm;
	Base::Property<double> delta;

private:
	cv::Mat tmp;
};

}//: namespace CvFilter2D
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFilter2D", Processors::CvFilter2D::CvFilter2D_Processor)

#endif /* CVFILTER2D_PROCESSOR_HPP_ */

