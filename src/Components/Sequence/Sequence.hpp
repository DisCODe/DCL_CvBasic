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

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_img;


	/*!
	 * Event handler function.
	 */
	void onTrigger();

	/// Event handler.
	Base::EventHandler<Sequence> h_onTrigger;

	/*!
	 * Event handler function - moves image index to the next frame of the sequence.
	 */
	void onLoadNextImage();

	/// Event handler - moves image index to the next frame of the sequence.
	Base::EventHandler<Sequence> h_onLoadNextImage;


	/*!
	 * Event handler function - loads image from the sequence.
	 */
	void onLoadImage();

	/// Event handler - loads image from the sequence.
	Base::EventHandler<Sequence> h_onLoadImage;

	/*!
	 * Event handler function - reload the sequence.
	 */
	void onSequenceReload();

	/// Event handler - reload the sequence.
	Base::EventHandler<Sequence> h_onSequenceReload;

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

	/// Flag indicating thethwe the image was already loaded or not.
	bool trig;

	/// Directory containing the images sequence.
	Base::Property<std::string> prop_directory;

	/// Files pattern (regular expression).
	Base::Property<std::string> prop_pattern;

	/// Loading mode: triggered vs continous.
	Base::Property<bool> prop_triggered;

	/// Loading mode: images loaded in the loop.
	Base::Property<bool> prop_loop;

	/// Sort image sequence by their names.
	Base::Property<bool> prop_sort;

	/// Working mode: iterative vs constant.
	Base::Property<bool> prop_iterate;

	/// ???
	//Base::Property<bool> prop_prefetch;

};

}//: namespace Sequence
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_COMPONENT("Sequence", Sources::Sequence::Sequence)

#endif /* SEQUENCE_HPP_ */
