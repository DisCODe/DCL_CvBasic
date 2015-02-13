/*!
 * \file
 * \brief 
 * \author Maciej Stefanczyk
 */

#ifndef PERSPECTIVETRANSFORM_HPP_
#define PERSPECTIVETRANSFORM_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>

#include "Types/Points.hpp"


namespace Processors {
namespace PerspectiveTransform {

/*!
 * \class PerspectiveTransform
 * \brief PerspectiveTransform processor class.
 *
 * 
 */
class PerspectiveTransform: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	PerspectiveTransform(const std::string & name = "PerspectiveTransform");

	/*!
	 * Destructor
	 */
	virtual ~PerspectiveTransform();

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


	// Input data streams
	Base::DataStreamIn<cv::Point2f, Base::DataStreamBuffer::Newest> in_point;
	Base::DataStreamIn<std::vector<cv::Point2f>, Base::DataStreamBuffer::Newest > in_points;
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_homography;
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_img;

	// Output data streams
	Base::DataStreamOut<cv::Point2f> out_point;
	Base::DataStreamOut<std::vector<cv::Point2f> > out_points;
	Base::DataStreamOut<Types::Points> out_draw;
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers

	// Properties

	
	// Handlers
	void transformImage();
	void transformPoint();
	void transformPoints();
	void clearHomography();
	
	cv::Mat H;
};

} //: namespace PerspectiveTransform
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("PerspectiveTransform", Processors::PerspectiveTransform::PerspectiveTransform)

#endif /* PERSPECTIVETRANSFORM_HPP_ */
