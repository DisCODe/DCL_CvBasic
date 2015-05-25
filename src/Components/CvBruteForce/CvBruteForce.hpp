/*!
 * \file
 * \brief 
 * \author Jan Figat
 * \e-mail jan.figat@gmail.com
 */

#ifndef CVBRUTEFORCE_HPP_
#define CVBRUTEFORCE_HPP_

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
namespace CvBruteForce {

using namespace cv;

/*!
 * \class CvBruteForce
 * \brief CvBruteForce processor class.
 *
 * CvBruteForce processor.
 */
class CvBruteForce: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvBruteForce(const std::string & name = "CvBruteForce");

	/*!
	 * Destructor
	 */
	virtual ~CvBruteForce();

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

	/// Input data stream containing extracted features.
	Base::DataStreamIn <Types::Features, Base::DataStreamBuffer::Newest> in_features0;
	Base::DataStreamIn <Types::Features, Base::DataStreamBuffer::Newest> in_features1;

	/// Input data streams containing features descriptors
	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest> in_descriptors0;
	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest> in_descriptors1;

	/// Input data streams containing images
	Base::DataStreamIn <Mat, Base::DataStreamBuffer::Newest> in_img0;
	Base::DataStreamIn <Mat, Base::DataStreamBuffer::Newest> in_img1;

	/// Output data stream - "matching" image
	Base::DataStreamOut <Mat> out_img;
	Base::DataStreamOut < std::vector<DMatch> > out_matches;

	/// Flag: automatic distance recalculation.
	Base::Property<bool> distance_recalc;

	/// Flag: printing matching statistics.
	Base::Property<bool> print_stats;

	/// Minimal distance between two features so they will be classified as match.
	Base::Property<double> dist;

};

} //: namespace CvBruteForce
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvBruteForce", Processors::CvBruteForce::CvBruteForce)

#endif /* CVBRUTEFORCE_HPP_ */
