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

namespace Types {

class Circles : public Drawable
{
public:
	Circles();
	virtual ~Circles();

	std::vector<cv::Vec3f> circles;

	virtual void draw(cv::Mat& image, cv::Scalar color, int offsetX = 0, int offsetY = 0);

	virtual Drawable * clone();
};

}

#endif /* CIRCLES_HPP_ */
