/*!
 * \file
 * \brief 
 * \author Jan Figat
 * \e-mail jan.figat@gmail.com
 */

#ifndef CVFREAK_HPP_
#define CVFREAK_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Processors {
namespace CvFreak {

using namespace cv;

/*!
 * \class CvFreak
 * \brief CvFreak processor class.
 *
 */
class CvFreak: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvFreak(const std::string & name = "CvFreak");

	/*!
	 * Destructor
	 */
	virtual ~CvFreak();

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
	Base::EventHandler <CvFreak> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

    /// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;


};

} //: namespace CvFreak
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFreak", Processors::CvFreak::CvFreak)

#endif /* CVFREAK_HPP_ */
