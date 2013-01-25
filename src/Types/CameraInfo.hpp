/*
 * CameraInfo.hpp
 *
 *  Created on: 30-08-2012
 *      Author: tkornuta
 */

#ifndef CAMERAINFO_HPP_
#define CAMERAINFO_HPP_

#include <opencv/cv.h>

namespace Types {

/*!
 * \class CameraInfo
 *
 * \brief Contains data regarding the utilized camera.
 *
 * \author tkornuta
 */
class CameraInfo {
public:
	CameraInfo() {
	}

	CameraInfo(cv::Size imageSize_) : imageSize(imageSize_) {
	}

	virtual ~CameraInfo() {}

	void setSize(cv::Size imageSize_) {
		imageSize = imageSize_;
	}

	cv::Size  size() {
		return imageSize;
	}

protected:
	// Size of the returned image.
	cv::Size imageSize;
};

} //: namespace Types

#endif /* CAMERAINFO_HPP_ */
