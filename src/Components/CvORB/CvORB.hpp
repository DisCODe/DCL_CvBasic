/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvORB_HPP_
#define CvORB_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"


namespace Processors {
namespace CvORB {

using namespace cv;

/*!
 * \class CvORB
 * \brief CvORB processor class.
 *
 * CvORB processor.
 */
class CvORB: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvORB(const std::string & name = "CvORB");

	/*!
	 * Destructor
	 */
	virtual ~CvORB();

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
	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;

	/// The maximum number of features to retain
	Base::Property<int> nfeatures;
};

} //: namespace CvORB
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvORB", Processors::CvORB::CvORB)

#endif /* CvORB_HPP_ */
