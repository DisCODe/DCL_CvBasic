/*
 * Circles.hpp
 *
 *  Created on: 20-05-2011
 *      Author: x
 */

#ifndef CIRCLES_HPP_
#define CIRCLES_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

#include "Types/Drawable.hpp"

using namespace cv;

namespace Types {

class Circles : public Drawable
{
public:
	Circles() {};
	virtual ~Circles() {};

	std::vector<cv::Vec3f> circles;

	virtual void draw(cv::Mat& image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
		for (unsigned int i = 0; i < circles.size(); ++i) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// draw the circle center
			circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// draw the circle outline
			circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}
	}

	virtual Drawable * clone() {
		return new Circles(*this);
	}
};

}

#endif /* CIRCLES_HPP_ */
