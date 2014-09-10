/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVMOMENTS_HPP_
#define CVMOMENTS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Processors {
namespace CvMoments {

/*!
 * \class CvMoments
 * \brief CvMoments processor class.
 *
 * CvMoments processor.
 */
class CvMoments: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvMoments(const std::string & name = "CvMoments");

	/*!
	 * Destructor
	 */
	virtual ~CvMoments();

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
	void onNewContours();

	/// Input data stream
	Base::DataStreamIn <vector<vector<Point> > > in_contours;

	/// Output data stream containing extracted moments
	Base::DataStreamOut <vector<Moments> > out_moments;

};

} //: namespace CvMoments
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvMoments", Processors::CvMoments::CvMoments)

#endif /* CVMOMENTS_HPP_ */
