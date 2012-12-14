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
#include "Panel_Empty.hpp"
#include "Property.hpp"


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

class MatrixTranslator {
public:
    static cv::Mat fromStr(const std::string & s) {
        std::vector<double> values;

        typedef vector< string > split_vector_type;

        split_vector_type rows; // #2: Search for tokens
        boost::split( rows, s, boost::is_any_of(";"), boost::token_compress_on );

        std::vector<split_vector_type> mat;
        mat.resize(rows.size());
        for (int i = 0; i < rows.size(); ++i) {
        	boost::split( mat[i], rows[i], boost::is_any_of(" ,"), boost::token_compress_on );
        }

        int r = rows.size();
        int c = mat[0].size();

        cv::Mat ret(r, c, CV_32FC1);
        for (int rr = 0; rr < r; ++rr) {
        	for (int cc = 0; cc < c; ++cc)
        		ret.at<float>(rr, cc) = boost::lexical_cast<float>(mat[rr][cc]);
        }

        return ret;
    }

    static std::string toStr(cv::Mat m) {
    	std::stringstream ss;
    	std::string delim = "";
        for(int r = 0; r < m.rows; ++r) {
        	for(int c = 0; c < m.cols; ++c) {
        		ss << m.at<float>(r, c) << " ";
        	}
        	ss << delim;
        	delim = ";";
        }

        return ss.str();
    }
};

/**
 * CvUndistort properties.
 */
/*struct CvUndistortProps : public Base::Props
{
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from

	virtual void load(const ptree & pt)
	{
		LOG(LTRACE) << "loading camera parameters.\n";
		boost::numeric::ublas::matrix <double> cameraMatrixUblas = str2mat(pt.get <std::string> ("cameraMatrix"), 3, 3);
		cameraMatrix = cv::Mat(3, 3, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				cameraMatrix.at <float> (i, j) = cameraMatrixUblas(i, j);
				LOG(LDEBUG) << "cameraMatrix(" << i << ", " << j << "): " << cameraMatrix.at <float> (i, j) << endl;
			}
		}

		boost::numeric::ublas::matrix <double> distCoeffsUblas = str2mat(pt.get <std::string> ("distCoeffs"), 1, 5);
		distCoeffs = cv::Mat(1, 5, CV_32F);
		for (int j = 0; j < 5; ++j) {
			distCoeffs.at <float> (0, j) = distCoeffsUblas(0, j);
			LOG(LDEBUG) << "distCoeffs(" << 0 << ", " << j << "): " << distCoeffs.at <float> (0, j) << endl;
		}
	}

	!
	 * Save settings
	 *
	 * @param pt root property tree to save settings

	virtual void save(ptree & pt)
	{
	}
};*/

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

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep();

private:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <CvUndistort_Processor> h_onNewImage;
	Base::Event *newUndistortedImage;

	Base::DataStreamIn <cv::Mat> in_img;
	Base::DataStreamOut <cv::Mat> out_img;

	Base::Property<cv::Mat, MatrixTranslator> cameraMatrix;
	Base::Property<cv::Mat, MatrixTranslator> distCoeffs;

	cv::Mat map1;
	cv::Mat map2;
	int interpolation;
};

} // namespace CvUndistort

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CvUndistort", Processors::CvUndistort::CvUndistort_Processor, Common::Panel_Empty)

#endif /* CVUNDISTORT_PROCESSOR_HPP_ */
