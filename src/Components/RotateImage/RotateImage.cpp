/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "RotateImage.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace RotateImage {

RotateImage::RotateImage(const std::string & name) :
		Base::Component(name),
//		center("center", 0.0),
		prop_angle("angle", 0.0),
		prop_scale("scale", 1.0)
{
	// Register properties.
	registerProperty(prop_angle);
	registerProperty(prop_scale);
}

RotateImage::~RotateImage() {
}

void RotateImage::prepareInterface() {
	// Register data streams.
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);

	// Register event handler.
	registerHandler("onNewImage", boost::bind(&RotateImage::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool RotateImage::onInit() {

	return true;
}

bool RotateImage::onFinish() {
	return true;
}

bool RotateImage::onStop() {
	return true;
}

bool RotateImage::onStart() {
	return true;
}


void RotateImage::onNewImage()
{
	CLOG(LTRACE) << "RotateImage::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat img_out;
		cv::Mat rot_mat( 2, 3, CV_32FC1 );

		// Compute a rotation matrix with respect to the center of the image.
		cv::Point center = cv::Point( img.cols/2, img.rows/2 );
   		rot_mat = getRotationMatrix2D( center, prop_angle, prop_scale );

		  /// Rotate the image.
		cv::warpAffine( img, img_out, rot_mat, img_out.size() );

		out_img.write(img_out);
	} catch (...) {
		CLOG(LERROR) << "RotateImage::onNewImage failed\n";
	}
}


} //: namespace RotateImage
} //: namespace Processors
