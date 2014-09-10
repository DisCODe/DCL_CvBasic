/*!
 * \file
 * \brief 
 * \author Tomek Kornuta
 */

#ifndef HSVLUT_HPP_
#define HSVLUT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Processors {
namespace HSVLUT {

/*!
 * \class HSVLUT
 * \brief HSVLUT processor class.
 *
 * HSVLUT processor.
 */
class HSVLUT: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	HSVLUT(const std::string & name = "HSVLUT");

	/*!
	 * Destructor
	 */
	virtual ~HSVLUT();

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

	Base::Property<int> m_hue_threshold_low;
	Base::Property<int> m_hue_threshold_high;
	Base::Property<int> m_sat_threshold_low;
	Base::Property<int> m_sat_threshold_high;
	Base::Property<int> m_val_threshold_low;
	Base::Property<int> m_val_threshold_high;

};

} //: namespace HSVLUT
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("HSVLUT", Processors::HSVLUT::HSVLUT)

#endif /* HSVLUT_HPP_ */
