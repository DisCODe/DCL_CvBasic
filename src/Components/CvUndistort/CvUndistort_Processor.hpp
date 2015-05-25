/*!
 * \file CvUndistort_Processor.hpp
 * \brief Camera distortion correction component.
 * \date Oct 20, 2010
 * \author mboryn
 */

#ifndef CVUNDISTORT_PROCESSOR_HPP_
#define CVUNDISTORT_PROCESSOR_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <sstream>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Property.hpp"

#include <Types/CameraInfo.hpp>

/**
 * \defgroup CvUndistort CvUndistort
 * \ingroup Processors
 *
 * \brief Applies distortion correction to the image.
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 * \par Properties:
 *
 * \prop{cameraMatrix,boost::numeric::ublas::matrix \<double\> 3x3,""}
 * Camera matrix.
 * For example:
 *
\code
[3,3](
	(647.07647705, 0, 383.50000000),
	(0, 632.76348877, 287.50000000),
	(0, 0, 1)
)
\endcode
 *
 * \prop{distCoeffs,boost::numeric::ublas::matrix \<double\> 1x5,""}
 * Distortion correction coefficients.
 * For example:
 * \code
[1,5](
	(-0.51084775, 0.23313555, 4.50637517e-03, -6.86281081e-03, 4.94365674e-03)
)
	\endcode
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/camera_calibration_and_3d_reconstruction.html#cv-undistort
 * @{
 *
 * @}
 */

namespace Processors {

namespace CvUndistort {

/**
 * Component for distortion correction.
 */
class CvUndistort_Processor : public Base::Component
{
public:
	CvUndistort_Processor(const std::string& n);
	virtual ~CvUndistort_Processor();

	virtual void prepareInterface();

protected:
	/*!
	 * Method called when component is started
	 * \return true on success
	 */
	virtual bool onStart();

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop();

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit();

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish();

private:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	Base::DataStreamIn <cv::Mat> in_img;
	Base::DataStreamIn <Types::CameraInfo> in_camera_info;
	Base::DataStreamOut <cv::Mat> out_img;

	Types::CameraInfo camera_info;

	int interpolation;
	
	cv::Mat map1, map2, newK;
	
	Base::Property<bool> stereo;
	Base::Property<int> alpha;
	
	int last_alpha;
};

} // namespace CvUndistort

} // namespace Processors

REGISTER_COMPONENT("CvUndistort", Processors::CvUndistort::CvUndistort_Processor)

#endif /* CVUNDISTORT_PROCESSOR_HPP_ */
