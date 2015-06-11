/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
 * \author Tomek Kornuta, tkornuta@gmail.com
 */

#include "Sequence.hpp"

#include <opencv2/highgui/highgui.hpp>

namespace Sources {
namespace Sequence {

Sequence::Sequence(const std::string & n) :
	Base::Component(n),
	prop_directory("sequence.directory", std::string(".")),
	prop_pattern("sequence.pattern", std::string(".*\\.(jpg|png|bmp|yaml|yml)")),
	prop_sort("mode.sort", true),
	prop_loop("mode.loop", false),
	prop_auto_publish_image("mode.auto_publish_image", true),
	prop_auto_next_image("mode.auto_next_image", true),
	prop_auto_prev_image("mode.auto_prev_image", false)
{
	registerProperty(prop_directory);
	registerProperty(prop_pattern);
	registerProperty(prop_sort);
	registerProperty(prop_loop);
	registerProperty(prop_auto_publish_image);
	registerProperty(prop_auto_next_image);
	registerProperty(prop_auto_prev_image);

	CLOG(LTRACE) << "Constructed";
}

Sequence::~Sequence() {
	CLOG(LTRACE) << "Destroyed";
}


void Sequence::prepareInterface() {
	// Register streams.
	registerStream("out_img", &out_img);
	registerStream("out_end_of_sequence_trigger", &out_end_of_sequence_trigger);
	registerStream("in_publish_image_trigger", &in_publish_image_trigger);
	registerStream("in_next_image_trigger", &in_next_image_trigger);
	registerStream("in_prev_image_trigger", &in_prev_image_trigger);

	// Register handlers - loads image, NULL dependency.
	registerHandler("onLoadImage", boost::bind(&Sequence::onLoadImage, this));
	addDependency("onLoadImage", NULL);

	// Register handlers - next image, can be triggered manually (from GUI) or by new data present in_load_next_image_trigger dataport.
	// 1st version - manually.
	registerHandler("Next image", boost::bind(&Sequence::onLoadNextImage, this));

	// 2nd version - external trigger.
	registerHandler("onTriggeredLoadNextImage", boost::bind(&Sequence::onTriggeredLoadNextImage, this));
	addDependency("onTriggeredLoadNextImage", &in_next_image_trigger);

	// Register handlers - prev image, can be triggered manually (from GUI) or by new data present in_load_next_image_trigger dataport.
	// 1st version - manually.
	registerHandler("Previous image", boost::bind(&Sequence::onLoadPrevImage, this));

	// 2nd version - external trigger.
	registerHandler("onTriggeredLoadPrevImage", boost::bind(&Sequence::onTriggeredLoadPrevImage, this));
	addDependency("onTriggeredLoadPrevImage", &in_prev_image_trigger);

	// Register handlers - reloads sequence, triggered manually.
	registerHandler("Reload sequence", boost::bind(&Sequence::onSequenceReload, this));

	registerHandler("Publish Image", boost::bind(&Sequence::onPublishImage, this));

	registerHandler("onTriggeredPublishImage", boost::bind(&Sequence::onTriggeredPublishImage, this));
	addDependency("onTriggeredPublishImage", &in_publish_image_trigger);

}

bool Sequence::onInit() {
	CLOG(LTRACE) << "initialize\n";

	// Set indices.
	index = 0;
	previous_index = -1;

	// Initialize flags.
	next_image_flag = false;
	prev_image_flag = false;
	reload_sequence_flag = true;

	return true;
}

bool Sequence::onFinish() {
	CLOG(LTRACE) << "onFinish";
	return true;
}

void Sequence::onPublishImage() {
    CLOG(LTRACE) << "onPublishImage";

    publish_image_flag = true;
}

void Sequence::onTriggeredPublishImage() {
    CLOG(LTRACE) << "onTriggeredPublishImage";

    in_publish_image_trigger.read();

    publish_image_flag = true;
}

void Sequence::onLoadImage() {
	CLOG(LTRACE) << "onLoadImage";

	CLOG(LDEBUG) << "Before index=" << index << " previous_index=" << previous_index;

	
	if(reload_sequence_flag) {
		// Try to reload PCDSequence.
		if (!findFiles()) {
			CLOG(LERROR) << "There are no files matching the regular expression "
					<< prop_pattern << " in " << prop_directory;
		}
		index = 0;
		reload_sequence_flag = false;
	} else if (previous_index == -1) {
		// Special case - start!
			index = 0;
	} else {
		// Check triggering mode.
		if ((prop_auto_next_image) || (next_image_flag)) {
			index++;
		}//: if

		if ((prop_auto_prev_image) || (prev_image_flag)) {
			index--;
		}//: if

		// Anyway, reset flags.
		next_image_flag = false;
		prev_image_flag = false;

		// Check index range - first image.
		if (index <0){
			out_end_of_sequence_trigger.write(Base::UnitType());
			if (prop_loop) {
				index = files.size() -1;
				CLOG(LDEBUG) << "Sequence loop";
			} else {
				// Sequence ended - truncate index, but do not return image.
				index = 0;
				CLOG(LINFO) << "End of sequence";
				return;
			}//: else
		}//: if

		// Check index range - last image.
		if (index >= files.size()) {
			out_end_of_sequence_trigger.write(Base::UnitType());
			if (prop_loop) {
				index = 0;
				CLOG(LINFO) << "loop";
			} else {
				// Sequence ended - truncate index, but do not return image.
				index = files.size() -1;
				CLOG(LINFO) << "End of sequence";
				return;
			}//: else
		}//: if
	}//: else

	// Check whether there are any images loaded.
	if(files.empty()){
		CLOG(LNOTICE) << "Empty sequence!";
		return;
	}//: else

	// Check publishing flags.
	if(!prop_auto_publish_image && !publish_image_flag)
		return;
	publish_image_flag = false;

	CLOG(LDEBUG) << "After: index=" << index << " previous_index=" << previous_index ;

	try {
		CLOG(LNOTICE) << "Returning image (" << index << "): " << files[index] ;
		if (index == previous_index) {
			CLOG(LDEBUG) << "Returning previous image";
			// There is no need to load the image - return stored one.
			out_img.write(img);
			return;
		}//: if

		CLOG(LDEBUG) << "Loading image from file";
		// Get file extension.
		std::string ext = files[index].substr(files[index].rfind(".")+1);
		CLOG(LDEBUG) << "Extracted file extension " << ext;
		// Read depth from yaml.
		if ((ext == "xml") || (ext == "yaml") || (ext == "yml") || (ext == "gz")){
			cv::FileStorage file(files[index], cv::FileStorage::READ);
			file["img"] >> img;
		}
		else
			img = cv::imread(files[index], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

		CLOG(LINFO) <<"Image loaded properly from "<<files[index];
		previous_index = index;
		// Write image to the output port.
		out_img.write(img);

	} catch (...) {
		CLOG(LWARNING) << "Image reading failed! [" << files[index] << "]";
	}//: catch

}


void Sequence::onTriggeredLoadNextImage(){
	CLOG(LDEBUG) << "onTriggeredLoadNextImage - next image from the sequence will be loaded";
	in_next_image_trigger.read();
	next_image_flag = true;
}


void Sequence::onLoadNextImage(){
	CLOG(LDEBUG) << "onLoadNextImage - next image from the sequence will be loaded";
	next_image_flag = true;
}


void Sequence::onSequenceReload() {
	CLOG(LDEBUG) << "onSequenceReload";
	reload_sequence_flag = true;
}


void Sequence::onTriggeredLoadPrevImage(){
	CLOG(LDEBUG) << "onTriggeredLoadPrevImage - prev image from the sequence will be loaded";
	in_prev_image_trigger.read();
	prev_image_flag = true;
}


void Sequence::onLoadPrevImage(){
	CLOG(LDEBUG) << "onLoadPrevImage - prev image from the sequence will be loaded";
	prev_image_flag = true;
}


bool Sequence::onStart() {
	return true;
}

bool Sequence::onStop() {
	return true;
}

bool Sequence::findFiles() {
	files.clear();

	files = Utils::searchFiles(prop_directory, prop_pattern);

	if (prop_sort)
		std::sort(files.begin(), files.end());

	CLOG(LINFO) << "Sequence loaded.";
	BOOST_FOREACH(std::string fname, files)
		CLOG(LINFO) << fname;

	return !files.empty();
}

}//: namespace Sequence
}//: namespace Sources
