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
#include "opencv2/nonfree/features2d.hpp"
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

	/// Event handler.
	Base::EventHandler <CvORB> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;

	 // The maximum number of features to retain
        Base::Property<int> nfeatures;
    /*
        //Pyramid decimation ratio, greater than 1. scaleFactor==2 means the classical pyramid, where each next level has 4x less pixels than the previous, but such a big scale factor will degrade feature matching scores dramatically. On the other hand, too close to 1 scale factor will mean that to cover certain scale range you will need more pyramid levels and so the speed will suffer.
		Base::Property<float> scaleFactor;
		//This is size of the border where the features are not detected. It should roughly match the patchSize parameter.
		Base::Property<int> nlevels;
		//This is size of the border where the features are not detected. It should roughly match the patchSize parameter.
		Base::Property<int> edgeThreshold;
		//
		Base::Property<int> firstLevel;
		//The number of points that produce each element of the oriented BRIEF descriptor
		Base::Property<int> WTA_K;
		//The default HARRIS_SCORE means that Harris algorithm is used to rank features (the score is written to KeyPoint::score and is used to retain best nfeatures features); FAST_SCORE is alternative value of the parameter that produces slightly less stable keypoints, but it is a little faster to compute.
		Base::Property<int> scoreType;
		//size of the patch used by the oriented BRIEF descriptor. Of course, on smaller pyramid layers the perceived image area covered by a feature will be larger.
        Base::Property<int> patchSize;
    */
};

} //: namespace CvORB
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvORB", Processors::CvORB::CvORB)

#endif /* CvORB_HPP_ */
