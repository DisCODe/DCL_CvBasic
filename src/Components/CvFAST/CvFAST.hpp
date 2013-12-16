/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvFAST_HPP_
#define CvFAST_HPP_

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
namespace CvFAST {

using namespace cv;

/*!
 * \class CvFAST
 * \brief CvFAST processor class.
 *
 * CvFAST processor.
 */
class CvFAST: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
    CvFAST(const std::string & name = "CvFAST");

	/*!
	 * Destructor
	 */
    virtual ~CvFAST();

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
    Base::EventHandler <CvFAST> h_onNewImage;

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

} //: namespace CvFAST
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFAST", Processors::CvFAST::CvFAST)

#endif /* CvFAST_HPP_ */
