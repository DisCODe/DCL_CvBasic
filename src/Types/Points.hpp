/*!
 * \file Rectangle.hpp
 * \brief File containing Rectangle type
 */

#ifndef POINTS_HPP_
#define POINTS_HPP_

#include "Drawable.hpp"

#include <cv.h>

namespace Types {

class Points : public Drawable {
public:
	Points() {};

	Points(float x, float y) {
		m_points.push_back(cv::Point2f(x, y));
	}

	Points(const Points & rhs) : m_points(rhs.m_points) {};

	virtual ~Points() {}

	virtual void draw(cv::Mat & image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
		for (int i = 0; i < m_points.size(); ++i)
			cv::circle(image, m_points[i], 2, color, 2);
	}

	virtual Drawable * clone() {
		return new Points(*this);
	}
	
	void addPoint(cv::Point2f pt) {
		m_points.push_back(pt);
	}
	
	void setPoints(std::vector<cv::Point2f> pts) {
		m_points = pts;
	}

private:
	std::vector<cv::Point2f> m_points;
};

} //: namespace Types

#endif /* POINTS_HPP_ */
