/*!
 * \file
 * \brief 
 * \author Tomek Kornuta
 */

#ifndef RGBLUT_HPP_
#define RGBLUT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace RGBLUT {

/*!
 * \class RGBLUT
 * \brief RGBLUT processor class.
 *
 * RGBLUT processor.
 */
class RGBLUT: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	RGBLUT(const std::string & name = "RGBLUT");

	/*!
	 * Destructor
	 */
	virtual ~RGBLUT();

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

	/// Input image
	Base::DataStreamIn <cv::Mat> in_img;

	/// Output data stream - image.
	Base::DataStreamOut <cv::Mat> out_img;

private:
	cv::Mat tmp_img;

	Base::Property<int> m_red_threshold_low;
	Base::Property<int> m_red_threshold_high;
	Base::Property<int> m_green_threshold_low;
	Base::Property<int> m_green_threshold_high;
	Base::Property<int> m_blue_threshold_low;
	Base::Property<int> m_blue_threshold_high;

};

} //: namespace RGBLUT
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("RGBLUT", Processors::RGBLUT::RGBLUT)

#endif /* RGBLUT_HPP_ */
