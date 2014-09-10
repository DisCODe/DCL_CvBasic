/*!
 * \file
 * \brief 
 * \author Marta Lepicka
 */

#ifndef DRAWCSYSTEM_HPP_
#define DRAWCSYSTEM_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>

#include "Types/HomogMatrix.hpp"
#include "Types/CameraInfo.hpp"
#include "Types/DrawableContainer.hpp"
#include "Types/Line.hpp"

#include "Property.hpp"

namespace Processors {
namespace DrawCoordinateSystem {

/*!
 * \class DrawCoordinateSystem
 * \brief DrawCoordinateSystem processor class.
 *
 * 
 */
class DrawCoordinateSystem: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	DrawCoordinateSystem(const std::string & name = "DrawCoordinateSystem");

	/*!
	 * Destructor
	 */
	virtual ~DrawCoordinateSystem();

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
	Base::DataStreamIn <Types::HomogMatrix> in_homogMatrix;
	Base::DataStreamIn <cv::Mat> in_rvec;
	Base::DataStreamIn <cv::Mat> in_tvec;
	Base::DataStreamIn <Types::CameraInfo> in_camera_matrix;
	// Output data streams

	Base::DataStreamOut <Types::DrawableContainer> out_csystem;
	Base::DataStreamOut <std::vector<cv::Point2f> > out_impoints;

	void projectPoints();

};

} //: namespace DrawCoordinateSystem
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DrawCoordinateSystem", Processors::DrawCoordinateSystem::DrawCoordinateSystem)

#endif /* DRAWCSYSTEM_HPP_ */
