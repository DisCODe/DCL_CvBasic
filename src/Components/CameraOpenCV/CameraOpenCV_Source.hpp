/*!
 * \file CameraOpenCV_Source.hpp
 * \brief Class responsible for retrieving images from camera
 * \author mstefanc
 * \date 2010-06-10
 */


#ifndef CAMERAOPENCV_SOURCE_HPP_
#define CAMERAOPENCV_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Sources {
namespace CameraOpenCV {

using namespace cv;

/*!
 * \class CameraOpenCV_Source
 * \brief Class responsible for retrieving images from movies.
 */
class CameraOpenCV_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraOpenCV_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~CameraOpenCV_Source();

	/*!
	 * Prepares communication interface.
	 */
	virtual void prepareInterface();

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
	void onTrigger();

	/*!
	 * Event handler function.
	 */
	void onGrabFrame();

	/// Output data stream
	Base::DataStreamOut<Mat> out_img;

	/// Capture device
	VideoCapture cap;

	/// Movie frame
	Mat frame;

	bool trig;

	Base::Property<int> m_device;
	Base::Property<int> m_width;
	Base::Property<int> m_height;
	Base::Property<bool> m_triggered;

	void onDeviceCahnged(int old_device, int new_device);

	bool valid;

	bool m_change_device;

	void changeDevice();
};

}//: namespace CameraOpenCV
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_COMPONENT("CameraOpenCV", Sources::CameraOpenCV::CameraOpenCV_Source)

#endif /* CAMERAOPENCV_SOURCE_HPP_ */
