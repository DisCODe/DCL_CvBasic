/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVCANNY_HPP_
#define CVCANNY_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
//#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>



namespace Processors {
namespace CvCanny {

using namespace cv;

/*!
 * \class CvCanny
 * \brief CvCanny processor class.
 *
 * CvCanny processor.
 */
class CvCanny: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvCanny(const std::string & name = "CvCanny");

	/*!
	 * Destructor
	 */
	virtual ~CvCanny();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
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

	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;
	Base::DataStreamIn <Mat> in_mask;

	/// Output data stream - a normalized sum of input images
	Base::DataStreamOut <Mat> out_img;

	Base::Property<int> lowerThreshold;
	Base::Property<int> higherThreshold;
	Base::Property<int> kernelSize;

};

} //: namespace CvCanny
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvCanny", Processors::CvCanny::CvCanny)

#endif /* CVCANNY_HPP_ */
