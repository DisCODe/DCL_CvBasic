/*
 * GridPattern.hpp
 *
 *  Created on: 11-08-2014
 *  Author: tkornuta
 */

#ifndef GRIDPATTERN_HPP_
#define GRIDPATTERN_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "Object3D.hpp"

namespace Types {

namespace Objects3D {

class GridPattern : public Object3D
{
public:
	GridPattern(cv::Size patternSize) :
		patternSize(patternSize)
	{
	}
	GridPattern(const GridPattern& o) :
		Object3D(o),
		patternSize(o.patternSize)
	{
	}

	virtual ~GridPattern()
	{
	}

	virtual GridPattern* clone()
	{
		return new GridPattern(*this);
	}

	virtual void draw(cv::Mat& image, cv::Scalar color, int offsetX = 0, int offsetY = 0)
	{
		cv::Mat ip(imagePoints);
		cv::drawChessboardCorners(image, patternSize, ip, true);
	}
private:
	const cv::Size patternSize;
};

}

}

#endif /* GridPattern_HPP_ */
