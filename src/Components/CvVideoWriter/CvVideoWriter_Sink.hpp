/*!
 * \file CvVideoWriter_Sink.hpp
 * \brief
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef CVVIDEOWRITER_SINK_HPP_
#define CVVIDEOWRITER_SINK_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "Logger.hpp"
#include "Types/stream_OpenCV.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * \defgroup CvVideoWriter CvVideoWriter
 * \ingroup Sinks
 *
 * Writes all frames into movie file.
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 *
 * \par Properties:
 *
 * \prop{filename,string,"output.avi"}
 * Output file name
 * \prop{fourcc,string,"MJPG"}
 * Codec FOURCC code
 * \prop{size,cv::Size,640 480}
 * Movie frame size
 * \prop{fps,double,25.0}
 * Movie frame rate
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/reading_and_writing_images_and_video.html#VideoWriter
 *
 *
 * \par Task configuration template:
 *
 * \code
 * <Components>
 *   <Writer type="CvVideoWriter" thread="thread_name" group="group_name">
 *     <filename>output.avi</filename>
 *     <fourcc>MJPG</fourcc>
 *     <size>640 480</size>
 *     <fps>25.0</fps>
 *   </Writer>
 * </Components>
 *
 * <Events>
 *   <Event source="Component.Event" destination="Writer.onNewImage"/>
 * </Events>
 *
 * <DataStreams>
 *   <Writer>
 *     <in_img type="in" group="connection_name"/>
 *   </Writer>
 * </DataStreams>
 * \endcode
 *
 * @{
 *
 * @}
 */

namespace Sinks {
namespace CvVideoWriter {

using namespace cv;

class FourCCTranslator {
public:
	static int fromStr(const std::string & s) {
		if (s.length() != 4)
			return -1;

		return CV_FOURCC(s[0], s[1], s[2], s[3]);
	}

	static std::string toStr(int cc) {
		std::string ret = "XXXX";
		ret[0] = cc & 0xFF;
		ret[1] = (cc >> 8) & 0xFF;
		ret[2] = (cc >> 16) & 0xFF;
		ret[3] = (cc >> 24) & 0xFF;

		return ret;
	}
};

/*!
 * \class CvVideoWriter_Sink
 * \brief Writes all frames into movie file.
 */
class CvVideoWriter_Sink: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvVideoWriter_Sink(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvVideoWriter_Sink();

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
	 * Retrieves data from device.
	 */
	bool onStep();

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
	void onNewImage();

	/// Event handler.
	Base::EventHandler<CvVideoWriter_Sink> h_onNewImage;


	/// Image to be drawn
	Base::DataStreamIn<Mat> in_img;


	/// Video writer
	cv::VideoWriter writer;

	Base::Property<std::string> filename;
	Base::Property<int, FourCCTranslator> fourcc;
	Base::Property<int> width;
	Base::Property<int> height;
	Base::Property<double> fps;

};

}//: namespace CvVideoWriter
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvVideoWriter", Sinks::CvVideoWriter::CvVideoWriter_Sink)

#endif /* CVVIDEOWRITER_SINK_HPP_ */

