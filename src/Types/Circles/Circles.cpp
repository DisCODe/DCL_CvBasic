/*
 * Circles.cpp
 *
 *  Created on: 20-05-2011
 *      Author: x
 */

#include "Circles.hpp"

namespace Types {

using namespace cv;
using namespace std;

Circles::Circles()
{
	// TODO Auto-generated constructor stub

}

Circles::~Circles()
{
	// TODO Auto-generated destructor stub
}


void Circles::draw(cv::Mat& image, cv::Scalar color, int offsetX, int offsetY)
{
	for (unsigned int i = 0; i < circles.size(); ++i) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// draw the circle center
		circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// draw the circle outline
		circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
}

Drawable * Circles::clone()
{
	return new Circles(*this);
}

}
