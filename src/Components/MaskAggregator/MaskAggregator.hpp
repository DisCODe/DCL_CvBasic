/*!
 * \file
 * \brief 
 * \author Maciej Stefanczyk
 */

#ifndef MASKAGGREGATOR_HPP_
#define MASKAGGREGATOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace MaskAggregator {

/*!
 * \class MaskAggregator
 * \brief MaskAggregator processor class.
 *
 * 
 */
class MaskAggregator: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	MaskAggregator(const std::string & name = "MaskAggregator");

	/*!
	 * Destructor
	 */
	virtual ~MaskAggregator();

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
	Base::DataStreamIn<cv::Mat> in_mask;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_mask;

	// Handlers

	// Properties

	
	// Handlers
	void onNewImage();

};

} //: namespace MaskAggregator
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("MaskAggregator", Processors::MaskAggregator::MaskAggregator)

#endif /* MASKAGGREGATOR_HPP_ */
