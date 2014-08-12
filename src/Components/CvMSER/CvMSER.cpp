/*!
 * \file
 * \brief detector
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvMSER.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>


namespace Processors {
namespace CvMSER {

CvMSER::CvMSER(const std::string & name) :
        Base::Component(name)
{
}

CvMSER::~CvMSER() {
}

void CvMSER::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvMSER::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// img and output data streams.
	registerStream("in_img", &in_img);

	registerStream("out_contours", &out_contours);
	registerStream("out_img", &out_img);

}

bool CvMSER::onInit() {

	return true;
}

bool CvMSER::onFinish() {
	return true;
}

bool CvMSER::onStop() {
	return true;
}

bool CvMSER::onStart() {
	return true;
}

void CvMSER::onNewImage()
{
    LOG(LTRACE) << "CvMSER::onNewImage\n";
	try {
		// img: a RGB image.
		cv::Mat img = in_img.read();
		cv::Mat yuv,gray;
		cvtColor(img,yuv, COLOR_BGR2YCrCb);
		cvtColor(img, gray, COLOR_BGR2GRAY);
		vector<vector<Point> > regions;
		cv::MSER ms;
		ms(img, regions, cv::Mat());
		for (int i = 0; i < regions.size(); i++)
		    {
		        ellipse(img, fitEllipse(regions[i]), Scalar(255));
		    }

		// Write contours to the output.
		out_contours.write(regions);
		out_img.write(img);

	} catch (...) {
        LOG(LERROR) << "CvMSER::onNewImage failed\n";
	}
}



} //: namespace CvMSER
} //: namespace Processors
