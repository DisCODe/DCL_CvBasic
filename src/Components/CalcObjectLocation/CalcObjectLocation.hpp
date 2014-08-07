/*!
 * \file
 * \brief 
 * \author Marta Lepicka
 */

#ifndef CALCOBJECTLOCATION_HPP_
#define CALCOBJECTLOCATION_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>
#include "Types/HomogMatrix.hpp"
#include "Types/Objects3D/Object3D.hpp"
#include "Types/HomogMatrix.hpp"
#include "Types/CameraInfo.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

namespace Processors {
namespace CalcObjectLocation {

/*!
 * \class CalcObjectLocation
 * \brief CalcObjectLocation processor class.
 *
 * 
 */
class CalcObjectLocation: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CalcObjectLocation(const std::string & name = "CalcObjectLocation");

	/*!
	 * Destructor
	 */
	virtual ~CalcObjectLocation();

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
	Base::DataStreamIn<Types::HomogMatrix> in_homogMatrix;

	// Output data streams
	Base::DataStreamOut<Types::HomogMatrix> out_homogMatrix;

	// Handlers

	// Properties

	// Handlers
	Base::EventHandler<CalcObjectLocation> h_calculate;

	void calculate();
};

} //: namespace CalcObjectLocation
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CalcObjectLocation",
		Processors::CalcObjectLocation::CalcObjectLocation)

#endif /* CALCOBJECTLOCATION_HPP_ */
