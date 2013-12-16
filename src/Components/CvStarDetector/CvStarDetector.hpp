/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvSTARDETECTOR_HPP_
#define CvSTARDETECTOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Processors {
namespace CvStarDetector {

using namespace cv;

/*!
 * \class CvStarDetector
 * \brief CvStarDetector processor class.
 *
 * CvStarDetector processor.
 */
class CvStarDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
    CvStarDetector(const std::string & name = "CvStarDetector");

	/*!
	 * Destructor
	 */
    virtual ~CvStarDetector();

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
    Base::EventHandler <CvStarDetector> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;

	 // The maximum number of features to retain
        Base::Property<int> nfeatures;

};

} //: namespace CvStarDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvStarDetector", Processors::CvStarDetector::CvStarDetector)

#endif /* CvSTARDETECTOR_HPP_ */
