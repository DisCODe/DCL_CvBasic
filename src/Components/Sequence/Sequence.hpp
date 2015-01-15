/*!
 * \file Sequence.hpp
 * \brief Class responsible for retrieving images from image sequences - class declaration.
 */


#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include <vector>
#include <string>

#include <opencv2/core/core.hpp>

/**
 * \defgroup Sequence Sequence
 * \ingroup Sources
 *
 * \brief Image sequence provider.
 *
 * At the moment there is only one type of sequences
 * available, based on image filename pattern (regular expression) and directory,
 * in which files will be searched.
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
 * \event{endOfSequence}
 * Sequence has ended
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
 * \prop{directory,string,"."}
 * Directory, where fils will be searched
 * \prop{pattern,string,".*\.jpg"}
 * Regex pattern used for searching files
 * \prop{sort,bool,true}
 * If set, then found siles will be sorted in ascending order
 * \prop{prefetch,bool,false}
 * If set, all files will be loaded beforehand, otherwise images are loaded just before they are needed.
 * \prop{triggered,bool,false}
 * If set, new frames will be produced only after onTrigger event
 *
 *
 * \see \ref regex_basics
 *
 *
 * @{
 *
 * @}
 */

namespace Sources {
namespace Sequence {

/*!
 * \class Sequence
 * \brief Class responsible for retrieving images from image sequences.
 */
class Sequence : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	Sequence(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~Sequence();

	virtual void prepareInterface();

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

    /// Trigger - used for stream image in case of several sequences present.
    Base::DataStreamIn<Base::UnitType> in_stream_trigger;

    /// Trigger - used for loading next image in case of several sequences present.
    Base::DataStreamIn<Base::UnitType> in_next_image_trigger;

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_img;

    /*!
     * Event handler function - moves image index to the next frame of the sequence.
     */
    void onLoadNextImage();

    /*!
     * Event handler function - moves image index to the next frame of the sequence, externally triggered version.
     */
    void onTriggeredLoadNextImage();

    /*!
	 * Event handler function - loads image from the sequence.
	 */
	void onLoadImage();

	/*!
	 * Event handler function - reload the sequence.
	 */
	void onSequenceReload();

    /*!
     * Event handler function - stream image.
     */
    void onStreamImage();

    /*!
     * Event handler function - stream image, externally triggered version.
     */
    void onTriggeredStreamImage();

private:
	/**
	 * Fill list of files according to pattern
	 *
	 * \return true, if there is at least one file found, false otherwise
	 */
	bool findFiles();

	/// list of files in sequence
	std::vector<std::string> files;

	std::vector<cv::Mat> images;

	/// current frame
	cv::Mat img;

	/// Index of current frame.
	int frame;

	/// Flag indicating whether the image should be streamed
	bool streaming_flag;

	/// Flag indicating whether the next image should loaded or not.
	bool next_image_flag;

	/// Flag indicating whether the sequence should be reloaded or not.
	bool reload_flag;


	/// Directory containing the images sequence.
	Base::Property<std::string> prop_directory;

	/// Files pattern (regular expression).
	Base::Property<std::string> prop_pattern;

	/// Streaming mode: auto vs triggered.
	Base::Property<bool> prop_auto_streaming;

	/// Next image loading mode: next vs triggered
	Base::Property<bool> prop_auto_next_image;

	/// Loading mode: images loaded in the loop.
	Base::Property<bool> prop_loop;

	/// Sort image sequence by their names.
	Base::Property<bool> prop_sort;

	/// Returns image right after start.
	Base::Property<bool> prop_read_on_init;

};

}//: namespace Sequence
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_COMPONENT("Sequence", Sources::Sequence::Sequence)

#endif /* SEQUENCE_HPP_ */
