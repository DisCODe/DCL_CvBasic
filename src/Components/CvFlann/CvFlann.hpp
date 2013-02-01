/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVFLANN_HPP_
#define CVFLANN_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>




namespace Processors {
namespace CvFlann {

using namespace cv;

/*!
 * \class CvFlann
 * \brief CvFlann processor class.
 *
 * CvFlann processor.
 */
class CvFlann: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvFlann(const std::string & name = "CvFlann");

	/*!
	 * Destructor
	 */
	virtual ~CvFlann();

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
	Base::EventHandler <CvFlann> h_onNewImage;

	/// Input data stream containing extracted features.
	Base::DataStreamIn <Types::Features> in_features0;
	Base::DataStreamIn <Types::Features> in_features1;

	/// Input data streams containing features descriptors
	Base::DataStreamIn <cv::Mat> in_descriptors0;
	Base::DataStreamIn <cv::Mat> in_descriptors1;

	/// Input data streams containing images
	Base::DataStreamIn <Mat> in_img0;
	Base::DataStreamIn <Mat> in_img1;

	/// Output data stream - "matching" image
	Base::DataStreamOut <Mat> out_img;

	/// Flag: automatic distance recalculation.
	Base::Property<bool> distance_recalc;

	/// Flag: printing matching statistics.
	Base::Property<bool> print_stats;

	/// Minimal distance between two features so they will be classified as match.
	Base::Property<double> dist;

};

} //: namespace CvFlann
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFlann", Processors::CvFlann::CvFlann)

#endif /* CVFLANN_HPP_ */
