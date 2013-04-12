/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVSIFT_HPP_
#define CVSIFT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>


namespace Processors {
namespace CvSIFT {

using namespace cv;

/*!
 * \class CvSIFT
 * \brief CvSIFT processor class.
 *
 * CvSIFT processor.
 */
class CvSIFT: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvSIFT(const std::string & name = "CvSIFT");

	/*!
	 * Destructor
	 */
	virtual ~CvSIFT();

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
	Base::EventHandler <CvSIFT> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;

};

} //: namespace CvSIFT
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvSIFT", Processors::CvSIFT::CvSIFT)

#endif /* CVSIFT_HPP_ */
