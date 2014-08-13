/*!
 * \file
 * \brief 
 * \author Marta Lepicka
 */

#ifndef CALCSTATISTICS_HPP_
#define CALCSTATISTICS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>
#include "Types/Objects3D/Object3D.hpp"

#include <Types/HomogMatrix.hpp>
#include <Types/CameraInfo.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

namespace Processors {
namespace CalcStatistics {

/*!
 * \class CalcStatistics
 * \brief CalcStatistics processor class.
 *
 * 
 */
class CalcStatistics: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CalcStatistics(const std::string & name = "CalcStatistics");

	/*!
	 * Destructor
	 */
	virtual ~CalcStatistics();

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
	Base::EventHandler<CalcStatistics> h_calculate;

	Types::HomogMatrix cumulatedHomogMatrix;
	cv::Mat_<double> cumulatedRvec;
	cv::Mat_<double> cumulatedTvec;
	cv::Mat_<double> cumulatedAxis;
	double cumulatedFi;

	cv::Mat_<double> avgRvec;
	cv::Mat_<double> avgTvec;
	cv::Mat_<double> avgAxis;
	double avgFi;
	int counter;

	void calculate();

};

} //: namespace CalcStatistics
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CalcStatistics", Processors::CalcStatistics::CalcStatistics)

#endif /* CALCSTATISTICS_HPP_ */
