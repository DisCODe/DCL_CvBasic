/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef SKELETONIZATION_HPP_
#define SKELETONIZATION_HPP_ 

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Processors {
namespace Skeletonization {

/*!
 * \class Skeletonization
 * \brief Skeletonization processor class.
 *
 * Skeletonization processor.
 */
class Skeletonization: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Skeletonization(const std::string & name = "Skeletonization");

	/*!
	 * Destructor
	 */
	virtual ~Skeletonization();

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

	/// Event handler.
	Base::EventHandler <Skeletonization> h_onNewImage;

	/// Input image
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream - image.
	Base::DataStreamOut <cv::Mat> out_img;

};

} //: namespace Skeletonization
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Skeletonization", Processors::Skeletonization::Skeletonization)

#endif /* SKELETONIZATION_HPP_ */
