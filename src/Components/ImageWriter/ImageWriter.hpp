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

    /*!
     * Event handler function - saves image when button is pressed.
     */
    void onSaveButtonPressed();

    /*!
     * Event handler function - saves images.
     */
    void onSaveTriggered();

    /// Trigger - used for saving received images.
    Base::DataStreamIn<Base::UnitType> in_save_trigger;

    /// Image to be saved.
	std::vector< Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> *> in_img;

	/// Event handlers
	std::vector< Base::EventHandler2* > handlers;

	Base::Property<string> directory;
	Base::Property<string> base_name;
	Base::Property<string> format;
	Base::Property<int> digits;
	Base::Property<int> count;

	/// Vector of image names.
	std::vector<std::string> base_names;

	/// Vector of image extensions.
	std::vector<std::string> formats;

	/// Handlers
	void write_image_N(int n);

	/// Number of image streams.
	std::vector<int> counts;

	/// Saving mode: continous vs triggered.
	Base::Property<bool> prop_auto_trigger;

    /// Flag indicating whether the set of images images should saved or not (trigger).
	std::vector<bool> save_flags;


};

} //: namespace ImageWriter
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ImageWriter", Processors::ImageWriter::ImageWriter)

#endif /* IMAGEWRITER_HPP_ */
