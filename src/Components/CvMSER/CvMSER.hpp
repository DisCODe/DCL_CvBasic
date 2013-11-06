/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvMSER_HPP_
#define CvMSER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace CvMSER {

using namespace cv;




/*!
 * \class CvMSER
 * \brief CvMSER processor class.
 *
 * CvMSER processor.
 */
class CvMSER: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvMSER(const std::string & name = "CvMSER");

	/*!
	 * Destructor
	 */
	virtual ~CvMSER();

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
	Base::EventHandler <CvMSER> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted contours
	Base::DataStreamOut <vector<vector<Point> > > out_contours;
	Base::DataStreamOut <cv::Mat> out_img;

	//parameters
    /*Base::Property<int> delta;
    Base::Property<int> minArea;
    Base::Property<int> maxArea;
    Base::Property<int> maxVariation;
    Base::Property<int> minDiversity;
*/
};

} //: namespace CvMSER
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvMSER", Processors::CvMSER::CvMSER)

#endif /* CvMSER_HPP_ */
