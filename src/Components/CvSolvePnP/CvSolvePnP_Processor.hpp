/*!
 * \file CvSolvePnP_Processor.hpp
 * \brief Position estimation component.
 * \date Oct 20, 2010
 * \author mboryn
 */

#ifndef CVSOLVEPNP_PROCESSOR_HPP_
#define CVSOLVEPNP_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "EventHandler.hpp"
#include "DataStream.hpp"
#include "Types/Objects3D/Object3D.hpp"
#include "Types/HomogMatrix.hpp"
#include "Types/CameraInfo.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "Property.hpp"

/**
 * \defgroup CvSolvePnP CvSolvePnP
 * \ingroup Processors
 *
 * \brief Maps object found on input image to 3D object with given object's model.
 *
  * \par Data streams:
 *
 * \streamin{in_object3d,Types::Objects3D::Object3D}
 * Input 3D object.
 *
 * \streamout{out_object3d,Types::Objects3D::Object3D}
 * Output 3D object with estimated pose.
 *
 * \par Events:
 *
 * \event{objectLocated}
 * New object has been located.
 *
 * \par Event handlers:
 *
 * \handler{onNewObject3D}
 * New 3D object arrived.
 *
 * \par Properties:
 *
 * \prop{cameraMatrix,boost::numeric::ublas::matrix \<double\> 3x3,""}
 * Camera matrix.
 * For example:
 * \code
[3,3](
	(647.07647705, 0., 383.50000000),
	(0., 632.76348877, 287.50000000),
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
 * \see http://opencv.willowgarage.com/documentation/cpp/camera_calibration_and_3d_reconstruction.html#cv-solvepnp
 * @{
 *
 * @}
 */


namespace Processors {
namespace CvSolvePnP {

class CvSolvePnP_Processor: public Base::Component
{
public:
	CvSolvePnP_Processor(const std::string & n);
	virtual ~CvSolvePnP_Processor();

	void prepareInterface();
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

	void onNewObject3D();

	Base::Property<double> prop_x;
	Base::Property<double> prop_y;
	Base::Property<double> prop_z;
	Base::Property<double> prop_roll;
	Base::Property<double> prop_pitch;
	Base::Property<double> prop_yaw;


	Base::DataStreamInPtr <Types::Objects3D::Object3D> in_object3d;
	Base::DataStreamIn <Types::CameraInfo> in_camerainfo;
	Base::DataStreamOut <Types::HomogMatrix> out_homogMatrix;
	Base::DataStreamOut <cv::Mat> out_rvec;
	Base::DataStreamOut <cv::Mat> out_tvec;
};

} // namespace CvSolvePnP

} // namespace Processors

REGISTER_COMPONENT("CvSolvePnP", Processors::CvSolvePnP::CvSolvePnP_Processor)

#endif /* CVSOLVEPNP_PROCESSOR_HPP_ */
