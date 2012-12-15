/*!
 * \file Movie_Source.hpp
 * \brief Class responsible for retrieving images from movies
 * \author mstefanc
 * \date 2010-05-14
 */


#ifndef MOVIE_SOURCE_HPP_
#define MOVIE_SOURCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * \defgroup Movie Movie
 * \ingroup Sources
 * \brief Movie reader, based on OpenCV.
 *
 *
 *
 * \par Data streams:
 *
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onTrigger}
 * Trigger new frame
 *
 *
 * \par Properties:
 *
 * \prop{filename,string,""}
 * Name of movie file
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/reading_and_writing_images_and_video.html#videocapture
 * @{
 *
 * @}
 */

namespace Sources {
namespace Movie {

using namespace cv;

/*!
 * \class Movie_Source
 * \brief Class responsible for retrieving images from movies.
 */
class Movie_Source : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Movie_Source(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Movie_Source();

	void prepareInterface();

protected:
	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();



	/*!
	 * Event handler function.
	 */
	void onTrigger();

	/// Event handler.
	Base::EventHandler<Movie_Source> h_onTrigger;


	/*!
	 * Event handler function.
	 */
	void onStep();

	/// Event handler.
	Base::EventHandler<Movie_Source> h_onStep;


	/// Output data stream
	Base::DataStreamOut<Mat> out_img;

	/// Capture device
	VideoCapture cap;

	/// Movie frame
	Mat frame;

	bool trig;

	Base::Property<std::string> filename;
	Base::Property<bool> triggered;
};

}//: namespace Movie
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_COMPONENT("Movie", Sources::Movie::Movie_Source)

#endif /* MOVIE_SOURCE_HPP_ */
