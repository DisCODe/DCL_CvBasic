/*!
 * \file
 * \brief 
 * \author Maciej Stefańczyk [maciek.slon@gmail.com]
 */

#ifndef IMAGEWRITER_HPP_
#define IMAGEWRITER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Processors {
namespace ImageWriter {

/*!
 * \class ImageWriter
 * \brief ImageWriter processor class.
 *
 * Image writer
 */
class ImageWriter: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ImageWriter(const std::string & name = "ImageWriter");

	/*!
	 * Destructor
	 */
	virtual ~ImageWriter();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
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

	/// Image to be drawn
	std::vector< Base::DataStreamIn<cv::Mat> *> in_img;

	/// Event handlers
	std::vector< Base::EventHandler2* > handlers;

	Base::Property<string> directory;
	Base::Property<string> base_name;
	Base::Property<string> format;
	Base::Property<int> digits;
	Base::Property<int> count;
	
	std::vector<std::string> base_names;
	std::vector<std::string> formats;

	// Handlers
	void write_image_N(int n);

	std::vector<int> counts;

};

} //: namespace ImageWriter
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ImageWriter", Processors::ImageWriter::ImageWriter)

#endif /* IMAGEWRITER_HPP_ */
