/*
 * Features.hpp
 *
 *  Created on: 20-01-2013
 *      Author: tkornuta
 */

#ifndef FEATURES_HPP_
#define FEATURES_HPP_

#include "Drawable.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace Types {

class Features : public Drawable {
public:
	Features()
	{}

	Features(const Types::Features & _features) {
		features.insert(features.end(), _features.features.begin(), _features.features.end());
	};

	Features(const std::vector<cv::KeyPoint> & _features) {
		features.insert(features.end(), _features.begin(), _features.end());
	};

	virtual ~Features() {}

	virtual void draw(cv::Mat & image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
	    cv::drawKeypoints(image, features, image);
	}

	virtual Drawable * clone() {
		return new Features(*this);
	}

//private:
	std::vector<cv::KeyPoint> features;
};

} //: namespace Types



#endif /* FEATURES_HPP_ */
