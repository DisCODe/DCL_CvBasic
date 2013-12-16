/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvBRIEF_HPP_
#define CvBRIEF_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"


namespace Processors {
namespace CvBRIEF {

using namespace cv;

/*!
 * \class CvBRIEF
 * \brief CvBRIEF processor class.
 *
 * CvBRIEF processor.
 */
class CvBRIEF: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvBRIEF(const std::string & name = "CvBRIEF");

	/*!
	 * Destructor
	 */
	virtual ~CvBRIEF();

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
	Base::EventHandler <CvBRIEF> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Output data stream containing feature descriptors
	Base::DataStreamOut <cv::Mat> out_descriptors;



};

} //: namespace CvBRIEF
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvBRIEF", Processors::CvBRIEF::CvBRIEF)

#endif /* CvBRIEF_HPP_ */
