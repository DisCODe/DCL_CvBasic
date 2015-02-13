/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef SUM_HPP_
#define SUM_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
//#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace Sum {

using namespace cv;

/*!
 * \class Sum
 * \brief Sum processor class.
 *
 * Sum processor.
 */
class Sum: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Sum(const std::string & name = "Sum");

	/*!
	 * Destructor
	 */
	virtual ~Sum();

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

	/// Input data streams
	Base::DataStreamIn <Mat, Base::DataStreamBuffer::Newest> in_img1;
	Base::DataStreamIn <Mat, Base::DataStreamBuffer::Newest> in_img2;

	/// Output data stream - a normalized sum of input images
	Base::DataStreamOut <Mat> out_img;

	// A normalizer, by which the sum will be multipiled.
	Base::Property<double> norm;

private:
	cv::Mat tmp;

};

} //: namespace Sum
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Sum", Processors::Sum::Sum)

#endif /* SUM_HPP_ */
