/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef ROTATEIMAGE_HPP_
#define ROTATEIMAGE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace RotateImage {

/*!
 * \class RotateImage
 * \brief RotateImage processor class.
 *
 * RotateImage processor.
 */
class RotateImage: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	RotateImage(const std::string & name = "RotateImage");

	/*!
	 * Destructor
	 */
	virtual ~RotateImage();

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
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_img;

	/// Center of the rotation in the source image.
//	Base::Property<cv::Point2f> center;

	/// Rotation angle in degrees. Positive values mean counter-clockwise rotation (the coordinate origin is assumed to be the top-left corner).
	Base::Property<double> prop_angle;

	/// Isotropic scale factor.
	Base::Property<double> prop_scale;

};

} //: namespace RotateImage
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("RotateImage", Processors::RotateImage::RotateImage)

#endif /* ROTATEIMAGE_HPP_ */
