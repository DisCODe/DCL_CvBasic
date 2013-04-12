/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVSURF_HPP_
#define CVSURF_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>


namespace Processors {
namespace CvSURF {

using namespace cv;

/*!
 * \class CvSURF
 * \brief CvSURF processor class.
 *
 * CvSURF processor.
 */
class CvSURF: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvSURF(const std::string & name = "CvSURF");

	/*!
	 * Destructor
	 */
	virtual ~CvSURF();

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
	Base::EventHandler <CvSURF> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;

	// Hessian
	Base::Property<int> minHessian;

};

} //: namespace CvSURF
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvSURF", Processors::CvSURF::CvSURF)

#endif /* CVSURF_HPP_ */
