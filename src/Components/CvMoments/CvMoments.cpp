/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvMoments.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvMoments {

CvMoments::CvMoments(const std::string & name) :
		Base::Component(name)  {

}

CvMoments::~CvMoments() {
}

void CvMoments::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewContours.setup(this, &CvMoments::onNewContours);
	registerHandler("onNewContours", &h_onNewContours);
	addDependency("onNewContours", &in_contours);

	// Input and output data streams.
	registerStream("in_contours", &in_contours);
	registerStream("out_moments", &out_moments);
}

bool CvMoments::onInit() {

	return true;
}

bool CvMoments::onFinish() {
	return true;
}

bool CvMoments::onStop() {
	return true;
}

bool CvMoments::onStart() {
	return true;
}

void CvMoments::onNewContours()
{
	CLOG(LTRACE) << "CvMoments::onNewContours\n";
	try {
		// Input: a binary or edge image.
		vector<vector<Point> > contours = in_contours.read();

		// Compute moments.
		vector<Moments> mu(contours.size() );
		for( int i = 0; i < contours.size(); i++ )
		{
			mu[i] = moments( contours[i], false );

			CLOG(LINFO) << "[" << mu[i].m00 << "," << mu[i].m10 << "," << mu[i].m01 << "," << mu[i].m20 << "," << mu[i].m11 << "," << mu[i].m02 << "," << mu[i].m30 << "," << mu[i].m21 << "," << mu[i].m12 << "," << mu[i].m03
					//! central moments
					<< mu[i].mu20 << "," << mu[i].mu11 << "," << mu[i].mu02 << "," << mu[i].mu30 << "," << mu[i].mu21 << "," << mu[i].mu12 << "," << mu[i].mu03
					//! central normalized moments
					<< mu[i].nu20 << "," << mu[i].nu11 << "," << mu[i].nu02 << "," << mu[i].nu30 << "," << mu[i].nu21 << "," << mu[i].nu12 << "," << mu[i].nu03 <<"]";

		}

		// Write moments to the output.
		out_moments.write(mu);
	} catch (...) {
		CLOG(LERROR) << "CvMoments::onNewContours failed\n";
	}
}



} //: namespace CvMoments
} //: namespace Processors
