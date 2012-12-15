/*!
 * \file CvSplit_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVSPLIT_PROCESSOR_HPP_
#define CVSPLIT_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <vector>

#include <opencv2/core/core.hpp>

/**
 * \defgroup CvSplit CvSplit
 * \ingroup Processors
 *
 * Divides multi-channel array into several single-channel arrays
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img_N,cv::Mat}
 * Output images, where N is channel number, counting from 0
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * Image split
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
 * \prop{channels,int,3}
 * Number of channels of input image
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/operations_on_arrays.html?highlight=split#split
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvSplit {

using namespace cv;

/*!
 * \class CvSplit_Processor
 * \brief Divides multi-channel array into several single-channel arrays
 */
class CvSplit_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvSplit_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvSplit_Processor();

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

	/// Event handler.
	Base::EventHandler <CvSplit_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Output data stream - image split into channels
	std::vector< Base::DataStreamOut<cv::Mat> * > out_img;

	/// Channels
	std::vector<Mat> mv;

	/// Threshold properties
	Base::Property<int> channels;
};

}//: namespace CvSplit
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvSplit", Processors::CvSplit::CvSplit_Processor, Common::Panel_Empty)

#endif /* CVSPLIT_PROCESSOR_HPP_ */

