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
	prop_pattern("sequence.pattern", std::string(".*\\.(jpg|png|bmp)")),
	prop_sort("mode.sort", true),
	//prop_prefetch("mode.prefetch", false),
	prop_triggered("mode.triggered", false),
	prop_iterate("mode.iterate", true),
	prop_loop("mode.loop", false)
{
	registerProperty(prop_directory);
	registerProperty(prop_pattern);
	registerProperty(prop_sort);
	//registerProperty(prop_prefetch);
	registerProperty(prop_triggered);
	registerProperty(prop_iterate);
	registerProperty(prop_loop);

	// Set first frame index number.
	if (prop_iterate)
		frame = -1;
	else
		frame = 0;

	trig = true;

	CLOG(LTRACE) << name() << ": constructed";
}

Sequence::~Sequence() {
	CLOG(LTRACE) << name() << ": destroyed";
}


void Sequence::prepareInterface() {
	// Register handlers.
	h_onTrigger.setup(this, &Sequence::onTrigger);
	registerHandler("Trigger", &h_onTrigger);

	h_onLoadImage.setup(this, &Sequence::onLoadImage);
	registerHandler("onLoadImage", &h_onLoadImage);

	h_onLoadNextImage.setup(this, &Sequence::onLoadNextImage);
	registerHandler("Load next image", &h_onLoadNextImage);

	h_onSequenceReload.setup(this, &Sequence::onSequenceReload);
	registerHandler("Reload sequence", &h_onSequenceReload);

	// Register streams.
	registerStream("out_img", &out_img);

	// Add dependencies.
	addDependency("onLoadImage", NULL);
}

bool Sequence::onInit() {
	CLOG(LTRACE) << "Sequence::initialize\n";

	if (!findFiles()) {
		CLOG(LERROR) << name() << ": There are no files matching regex "
				<< prop_pattern << " in " << prop_directory;
		return false;
	}

	return true;
}

bool Sequence::onFinish() {
	CLOG(LTRACE) << "Sequence::finish\n";

	return true;
}

void Sequence::onLoadImage() {
	CLOG(LDEBUG) << "Sequence::onLoadImage";

	// Check triggering mode.
	if (prop_triggered && !trig)
		return;
	trig = false;
	// Check iterate mode.
	if (prop_iterate)
		frame++;

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
		img = cv::imread(files[frame], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	} catch (...) {
		CLOG(LWARNING) << name() << ": image reading failed! ["
				<< files[frame] << "]";
	}

	// Write image to the output port.
	out_img.write(img);
}

void Sequence::onLoadNextImage(){
	CLOG(LDEBUG) << "Sequence::onLoadNextImage - next image from the sequence will be loaded";
	frame++;
}


void Sequence::onSequenceReload() {
	// Set first frame index number.
	if (prop_iterate)
		frame = -1;
	else
		frame = 0;
	// Try to load new sequence.
	if (!findFiles()) {
		CLOG(LERROR) << name() << ": There are no files matching regex "
				<< prop_pattern << " in " << prop_directory;
		frame = -1;
	}
}




bool Sequence::onStep() {

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

void Sequence::onTrigger() {
	trig = true;
}

}//: namespace Sequence
}//: namespace Sources
