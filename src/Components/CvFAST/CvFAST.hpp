/*!
 * \file
 * \brief 
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#ifndef CvFAST_HPP_
#define CvFAST_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Processors {
namespace CvFAST {

using namespace cv;

/*!
 * \class CvFAST
 * \brief CvFAST processor class.
 *
 * CvFAST processor.
 */
class CvFAST: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
    CvFAST(const std::string & name = "CvFAST");

	/*!
	 * Destructor
	 */
    virtual ~CvFAST();

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
    Base::EventHandler <CvFAST> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream containing extracted features
	Base::DataStreamOut <Types::Features> out_features;

	Base::Property<int> m_threshold;
};

} //: namespace CvFAST
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvFAST", Processors::CvFAST::CvFAST)

#endif /* CvFAST_HPP_ */
