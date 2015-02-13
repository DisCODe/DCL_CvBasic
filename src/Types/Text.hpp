/*!
 * \file Text.hpp
 * \brief File containing Text type
 */

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "Drawable.hpp"

#include <opencv2/opencv.hpp>

namespace Types {

class Text : public Drawable {
public:
	Text() {};

	Text(float x, float y, const std::string & text) {
		m_pos = cv::Point2f(x, y);
		m_text = text;
	}

	Text(const Text & rhs) : m_pos(rhs.m_pos), m_text(rhs.m_text) {};

	virtual ~Text() {}

	virtual void draw(cv::Mat & image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
		cv::putText(image, m_text, m_pos, cv::FONT_HERSHEY_SIMPLEX, 1.0, color);
	}

	virtual Drawable * clone() {
		return new Text(*this);
	}

private:
	cv::Point2f m_pos;
	std::string m_text;
};

} //: namespace Types

#endif /* TEXT_HPP_ */
