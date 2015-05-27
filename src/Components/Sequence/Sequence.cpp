/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
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
	prop_read_on_init("read_on_init", true) 
{
	registerProperty(prop_directory);
	registerProperty(prop_pattern);
	registerProperty(prop_sort);
	registerProperty(prop_loop);
	registerProperty(prop_auto_publish_image);
	registerProperty(prop_auto_next_image);
	registerProperty(prop_read_on_init);

	// Set first frame index number.
	if (prop_auto_next_image)
		frame = -1;
	else
		frame = 0;

	// Initialize flags
	next_image_flag = false;
	reload_flag = false;

	CLOG(LTRACE) << "constructed";
}

Sequence::~Sequence() {
	CLOG(LTRACE) << "destroyed";
}


void Sequence::prepareInterface() {
    // Register Publishs.
    registerStream("out_img", &out_img);
    registerStream("out_end_of_sequence_trigger", &out_end_of_sequence_trigger);
    registerStream("in_publish_image_trigger", &in_publish_image_trigger);
    registerStream("in_next_image_trigger", &in_next_image_trigger);

    // Register handlers - loads image, NULL dependency.
	registerHandler("onLoadImage", boost::bind(&Sequence::onLoadImage, this));
    addDependency("onLoadImage", NULL);

    // Register handlers - next image, can be triggered manually (from GUI) or by new data present in_load_next_image_trigger dataport.
    // 1st version - manually.
    registerHandler("Next image", boost::bind(&Sequence::onLoadNextImage, this));

    // 2nd version - external trigger.
    registerHandler("onTriggeredLoadNextImage", boost::bind(&Sequence::onTriggeredLoadNextImage, this));
    addDependency("onTriggeredLoadNextImage", &in_next_image_trigger);

    // Register handlers - reloads sequence, triggered manually.
    registerHandler("Reload sequence", boost::bind(&Sequence::onSequenceReload, this));

    registerHandler("Publish Image", boost::bind(&Sequence::onPublishImage, this));

    registerHandler("onTriggeredPublishImage", boost::bind(&Sequence::onTriggeredPublishImage, this));
    addDependency("onTriggeredPublishImage", &in_publish_image_trigger);

}

bool Sequence::onInit() {
	CLOG(LTRACE) << "Sequence::initialize\n";

	// Load files on first
	reload_flag = true;
	if (prop_read_on_init)
		next_image_flag = true;
	return true;
}

bool Sequence::onFinish() {
	CLOG(LTRACE) << "Sequence::finish\n";

	return true;
}

void Sequence::onPublishImage() {
    CLOG(LTRACE) << "Sequence::onPublishImage";

    publish_image_flag = true;
}

void Sequence::onTriggeredPublishImage() {
    CLOG(LTRACE) << "Sequence::onTriggeredPublishImage";

    in_publish_image_trigger.read();

    publish_image_flag = true;
}

void Sequence::onLoadImage() {
	CLOG(LDEBUG) << "Sequence::onLoadImage";

	if(reload_flag) {
		// Try to reload sequence.
		if (!findFiles()) {
			CLOG(LERROR) << "There are no files matching the regular expression "
					<< prop_pattern << " in " << prop_directory;
		}
		frame = -1;
		reload_flag = false;
	}


	// Check whether there are any images loaded.
	if(files.empty())
		return;

	// Check Publishing
	if(!prop_auto_publish_image && !publish_image_flag)
		return;

	publish_image_flag = false;

	// Check triggering mode.
	if ((prop_auto_next_image) || (!prop_auto_next_image && next_image_flag))
		frame++;
	
	// Anyway, reset flag.
	next_image_flag = false;

	// Check frame number.
	if (frame <0)
		frame = 0;
	// Check the size of the dataset.
	if (frame >= files.size()) {
		out_end_of_sequence_trigger.write(Base::UnitType());
		if (prop_loop) {
			frame = 0;
			CLOG(LINFO) << "loop";
		} else {
			frame = files.size() -1;
			CLOG(LINFO) << "end of sequence";
			return;
		}

	}

	CLOG(LINFO) << "Sequence: reading image " << files[frame];
	try {
		// Get file extension.
		std::string ext = files[frame].substr(files[frame].rfind(".")+1);
		CLOG(LDEBUG) << "Extracted file Extension " << ext;
		// Read depth from yaml.
		if ((ext == "yaml") || (ext == "yml")){
			cv::FileStorage file(files[frame], cv::FileStorage::READ);
			file["img"] >> img;
		}
		else
			img = cv::imread(files[frame], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

		// Write image to the output port.
		out_img.write(img);
	} catch (...) {
		CLOG(LWARNING) << ": image reading failed! [" << files[frame] << "]";
	}

}


void Sequence::onTriggeredLoadNextImage(){
    CLOG(LDEBUG) << "Sequence::onTriggeredLoadNextImage - next image from the sequence will be loaded";
    in_next_image_trigger.read();
	next_image_flag = true;
}


void Sequence::onLoadNextImage(){
	CLOG(LDEBUG) << "Sequence::onLoadNextImage - next image from the sequence will be loaded";
	next_image_flag = true;
}


void Sequence::onSequenceReload() {
	CLOG(LDEBUG) << "Sequence::onSequenceReload";
	reload_flag = true;
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
