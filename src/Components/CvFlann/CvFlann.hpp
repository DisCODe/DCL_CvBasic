/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVFLANN_HPP_
#define CVFLANN_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>



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

	/// Input data streams
	Base::DataStreamIn <Mat> in_img0;
	Base::DataStreamIn <Mat> in_img1;

	/// Output data stream - "matching" image
	Base::DataStreamOut <Mat> out_img;

	

};

} //: namespace CvFlann
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFlann", Processors::CvFlann::CvFlann)

#endif /* CVFLANN_HPP_ */
