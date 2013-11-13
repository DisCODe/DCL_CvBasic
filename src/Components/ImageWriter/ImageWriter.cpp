/*!
 * \file
 * \brief
 * \author Maciej Stefańczyk [maciek.slon@gmail.com]
 */

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

#include "ImageWriter.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Processors {
namespace ImageWriter {

ImageWriter::ImageWriter(const std::string & name) :
		Base::Component(name),
		directory("directory", std::string(".")),
		base_name("base_name", std::string("img")),
		format("format", std::string("png")),
		digits("digits", 2),
		count("count", 1){

	registerProperty(directory);
	registerProperty(base_name);
	registerProperty(format);
	registerProperty(digits);
	registerProperty(count);
}

ImageWriter::~ImageWriter() {
}

void ImageWriter::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	// Register data streams

	// Register handlers
	Base::EventHandler2 * hand;
	for (int i = 0; i < count; ++i) {
		char id = '0'+i;
		hand = new Base::EventHandler2;
		hand->setup(boost::bind(&ImageWriter::write_image_N, this, i));
		handlers.push_back(hand);
		registerHandler(std::string("write_image_")+id, hand);

		Base::DataStreamIn<cv::Mat> * stream = new Base::DataStreamIn<cv::Mat>;
		in_img.push_back(stream);
		registerStream( std::string("in_img_")+id, (Base::DataStreamInterface*)(in_img[i]));
		addDependency(std::string("write_image_")+id, stream);
	}


	// register aliases for first handler and streams
	registerHandler("write_image", handlers[0]);
	registerStream("in_img", in_img[0]);

	counts.resize(count, 0);

	std::string t = base_name;
	boost::split(base_names, t, boost::is_any_of(","));
	if ( (base_names.size() == 1) && (count > 1) ) base_names.clear();
	for (int i = base_names.size(); i < count; ++i) {
		char id = '0' + i;
		base_names.push_back(std::string(base_name) + id);
	}

	std::string f = format;
	boost::split(formats, f, boost::is_any_of(","));
	if ( (formats.size() == 1) && (count > 1) ) formats.clear();
	for (int i = formats.size(); i < count; ++i) {
		formats.push_back(format);
	}

}

bool ImageWriter::onInit() {
	return true;
}

bool ImageWriter::onFinish() {
	return true;
}

bool ImageWriter::onStop() {
	return true;
}

bool ImageWriter::onStart() {
	return true;
}

void ImageWriter::write_image_N(int n) {
	CLOG(LTRACE) << name() << "::onNewImage(" << n << ")";

	boost::posix_time::ptime tm = boost::posix_time::microsec_clock::local_time();

	try {
		if(!in_img[n]->empty()){
			counts[n] = counts[n] + 1;
			std::stringstream ss;
			ss << std::setw(digits) << std::setfill('0') << counts[n];
			//std::string fname = std::string(directory) + "/" + base_names[n] + boost::lexical_cast<std::string>(counts[n]) + "." + formats[n];
			std::string fname = std::string(directory) + "/" + boost::posix_time::to_iso_extended_string(tm) + "_" + base_names[n] + "." + formats[n];
			cv::imwrite(fname, in_img[n]->read());
			CLOG(LNOTICE) << "Written: " << fname;
		}
	}
	catch(std::exception &ex) {
		CLOG(LERROR) << "ImageWriter::write_image_N failed: " << ex.what() << "\n";
	}
}



} //: namespace ImageWriter
} //: namespace Processors
