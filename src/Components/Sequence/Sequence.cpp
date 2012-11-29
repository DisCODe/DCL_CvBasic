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
	prop_directory("directory", std::string(".")),
	prop_pattern("pattern", std::string(".*\\.(jpg|png|bmp)")),
	prop_sort("sort", true),
	prop_prefetch("prefetch", false),
	prop_triggered("triggered", false),
	prop_loop("loop", false)
{
	registerProperty(prop_directory);
	registerProperty(prop_pattern);
	registerProperty(prop_sort);
	registerProperty(prop_prefetch);
	registerProperty(prop_triggered);
	registerProperty(prop_loop);

	frame = 0;
	trig = true;

	CLOG(LTRACE) << name() << ": constructed";
}

Sequence::~Sequence() {
	CLOG(LTRACE) << name() << ": destroyed";
}


void Sequence::prepareInterface() {
	h_onTrigger.setup(this, &Sequence::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	h_onNextImage.setup(this, &Sequence::onNextImage);
	registerHandler("onNextImage", &h_onNextImage);

	newImage = registerEvent("newImage");
	endOfSequence = registerEvent("endOfSequence");

	registerStream("out_img", &out_img);

	addDependency("onNextImage", NULL);
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

void Sequence::onNextImage() {
	CLOG(LDEBUG) << "Sequence::onNextImage";

	if (prop_triggered && !trig)
		return;

	trig = false;

	if (frame >= files.size()) {
		if (prop_loop) {
			frame = 0;
			CLOG(LINFO) << name() << ": loop";
			endOfSequence->raise();
		} else {
			CLOG(LINFO) << name() << ": end of sequence";
			endOfSequence->raise();
			return;
		}
	}

	CLOG(LTRACE) << "Sequence: reading image " << files[frame];
	try {
		img = cv::imread(files[frame++], -1);
	} catch (...) {
		CLOG(LWARNING) << name() << ": image reading failed! ["
				<< files[frame - 1] << "]";
	}

	out_img.write(img);
	//newImage->raise();
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
