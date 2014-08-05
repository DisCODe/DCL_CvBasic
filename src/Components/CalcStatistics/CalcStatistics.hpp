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
	Base::DataStreamIn <cv::Mat> in_rvec;
	Base::DataStreamIn <cv::Mat> in_tvec;
	Base::DataStreamIn <cv::Mat> in_rvec_d;
	Base::DataStreamIn <cv::Mat> in_tvec_d;

	// Output data streams

	// Handlers

	// Properties

	
	// Handlers
	Base::EventHandler <CalcStatistics> h_calculate;
	Base::EventHandler <CalcStatistics> h_calculateDiff;

	void calculate();
	void calculateDiff();

	cv::Mat tvec;
	cv::Mat rvec;

	cv::Mat tvec_d;
	cv::Mat rvec_d;


};

} //: namespace CalcStatistics
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CalcStatistics", Processors::CalcStatistics::CalcStatistics)

#endif /* CALCSTATISTICS_HPP_ */
