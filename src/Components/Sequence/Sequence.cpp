/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
 */

#include "Sequence.hpp"

#include "Utils.hpp"

#include <algorithm>
#include <boost/foreach.hpp>

#include <opencv2/highgui/highgui.hpp>

namespace Sources {
namespace Sequence {

Sequence::Sequence(const std::string & n) :
	Base::Component(n),
	prop_directory("sequence.directory", std::string(".")),
	prop_pattern("sequence.pattern", std::string(".*\\.(jpg|png|bmp|yaml|yml)")),
	prop_sort("mode.sort", true),
	//prop_prefetch("mode.prefetch", false),
	prop_loop("mode.loop", false),
	prop_auto_trigger("mode.auto_trigger", true)
{
	registerProperty(prop_directory);
	registerProperty(prop_pattern);
	registerProperty(prop_sort);
	registerProperty(prop_loop);
	registerProperty(prop_auto_trigger);
	//registerProperty(prop_prefetch);

	// Set first frame index number.
	if (prop_auto_trigger)
		frame = -1;
	else
		frame = 0;

	// Initialize flags
	next_image_flag = false;
	reload_flag = false;

	CLOG(LTRACE) << name() << ": constructed";
}

Sequence::~Sequence() {
	CLOG(LTRACE) << name() << ": destroyed";
}


void Sequence::prepareInterface() {
    // Register streams.
    registerStream("out_img", &out_img);
    registerStream("in_trigger", &in_trigger);

    // Register handlers - loads image, NULL dependency.
	registerHandler("onLoadImage", boost::bind(&Sequence::onLoadImage, this));
    addDependency("onLoadImage", NULL);

    // Register handlers - next image, can be triggered manually (from GUI) or by new data present in_load_next_image_trigger dataport.
    // 1st version - manually.
    registerHandler("Next image", boost::bind(&Sequence::onLoadNextImage, this));

    // 2nd version - external trigger.
    registerHandler("onTriggeredLoadNextImage", boost::bind(&Sequence::onTriggeredLoadNextImage, this));
    addDependency("onTriggeredLoadNextImage", &in_trigger);

    // Register handlers - reloads sequence, triggered manually.
    registerHandler("Reload sequence", boost::bind(&Sequence::onSequenceReload, this));

}

bool Sequence::onInit() {
	CLOG(LTRACE) << "Sequence::initialize\n";

	// Load files on first
	reload_flag = true;

	return true;
}

bool Sequence::onFinish() {
	CLOG(LTRACE) << "Sequence::finish\n";

	return true;
}

void Sequence::onLoadImage() {
	CLOG(LDEBUG) << "Sequence::onLoadImage";

	if(reload_flag) {
		// Try to reload sequence.
		if (!findFiles()) {
			CLOG(LERROR) << name() << ": There are no files matching the regular expression "
					<< prop_pattern << " in " << prop_directory;
		}
		frame = -1;
		reload_flag = false;
	}


	// Check whether there are any images loaded.
	if(files.empty())
		return;

	// Check triggering mode.
	if ((prop_auto_trigger) || (!prop_auto_trigger && next_image_flag))
		frame++;
	// Anyway, reset flag.
	next_image_flag = false;

	// Check frame number.
	if (frame <0)
		frame = 0;
	// Check the size of the dataset.
	if (frame >= files.size()) {
		if (prop_loop) {
			frame = 0;
			CLOG(LINFO) << name() << ": loop";
			// TODO: endOfSequence->raise();
		} else {
			frame = files.size() -1;
			CLOG(LINFO) << name() << ": end of sequence";
			// TODO: endOfSequence->raise();
		}

	}

	CLOG(LTRACE) << "Sequence: reading image " << files[frame];
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
	} catch (...) {
		CLOG(LWARNING) << name() << ": image reading failed! ["
				<< files[frame] << "]";
	}

	// Write image to the output port.
	out_img.write(img);
}


void Sequence::onTriggeredLoadNextImage(){
    CLOG(LDEBUG) << "Sequence::onTriggeredLoadNextImage - next image from the sequence will be loaded";
    in_trigger.read();
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
