/*!
 * \file
 * \brief 
 * \author Maciej Stefanczyk
 */

#ifndef FEATUREDETECTOR_HPP_
#define FEATUREDETECTOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"


#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace FeatureDetector {

/*!
 * \class FeatureDetector
 * \brief FeatureDetector processor class.
 *
 * 
 */
class FeatureDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FeatureDetector(const std::string & name = "FeatureDetector");

	/*!
	 * Destructor
	 */
	virtual ~FeatureDetector();

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
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_img_A;
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_img_B;

	// Output data streams
	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features_A;
	Base::DataStreamOut <Types::Features> out_features_B;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors_A;
	Base::DataStreamOut <cv::Mat> out_descriptors_B;

	// Handlers

	// Properties
	Base::Property<int> orb_nfeatures;
	Base::Property<float> orb_scale_factor;
	Base::Property<int> orb_nlevels;
	Base::Property<int> orb_edge_threshold;
	Base::Property<int> orb_wta_k;
	Base::Property<int> orb_score_type;
	Base::Property<int> orb_patch_size;
	Base::Property<int> brisk_threshold;
	Base::Property<int> brisk_octaves;
	Base::Property<float> brisk_pattern_scale;
	Base::Property<std::string> active_extractor;

	
	// Handlers
	void onNewImageA();
	void onNewImageB();

};

} //: namespace FeatureDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FeatureDetector", Processors::FeatureDetector::FeatureDetector)

#endif /* FEATUREDETECTOR_HPP_ */
