/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVHARRIS_HPP_
#define CVHARRIS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace CvHarris {

using namespace cv;

/*!
 * \class CvHarris
 * \brief CvHarris processor class.
 *
 * CvHarris processor.
 */
class CvHarris: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvHarris(const std::string & name = "CvHarris");

	/*!
	 * Destructor
	 */
	virtual ~CvHarris();

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

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	/// Detector parameters
	Base::Property<int> blockSize;
	Base::Property<int> apertureSize;
	Base::Property<double> k;
	Base::Property<int> thresh;

};

} //: namespace CvHarris
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvHarris", Processors::CvHarris::CvHarris)

#endif /* CVHARRIS_HPP_ */
